#include "sender_thread.h"
#include "debugging.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

// don't use std::chrono_literals to remain compatible with C++11
static constexpr auto s_readTimeout = std::chrono::milliseconds{ 100 }; // 100ms;

class DataQueue {
public:
    size_t size() const {
        std::unique_lock<std::mutex> lock{ m_mutex };
        return m_queue.size();
    }

    // onced closed, the queue will not accept any new element, but the existing ones won't be removed
    // to do so, call clear() after you called close()
    void close() {
        m_isClosed = true;
    }

    // returns an empty string if nothing was read
    std::string readWithTimeout() {
        // using a condition variable we will be awaken right after a new message has been pushed
        // but if two messages are pushed very quickly we might miss the second notification
        // so we need to use a read timeout
        std::unique_lock<std::mutex> lock{ m_mutex };
        m_condvar.wait_for(lock, s_readTimeout);

        if (!m_queue.empty()) {
            std::string str = std::move(m_queue.front());
            m_queue.pop();
            return str;
        }
        return {}; // timed out
    }

    bool push(std::string str) {
        std::unique_lock<std::mutex> lock{ m_mutex };

        if (m_isClosed) {
            CPPCAPTURE_LOGERROR("discarding message because queue is closed");
            return false;
        }

        // check queue isn't full
        if (m_queue.size() == m_maxQueueSize) {
            CPPCAPTURE_LOGERROR("discarding message because queue is full");
            return false;
        }

        m_queue.emplace(std::move(str));
        m_condvar.notify_one();
        return true;
    }

    void clear() {
        std::unique_lock<std::mutex> lock{ m_mutex };
        m_queue = decltype(m_queue){};
        m_condvar.notify_one();
    }

private:
    const size_t m_maxQueueSize = 100;
    std::atomic<bool> m_isClosed{ false };
    std::queue<std::string> m_queue;
    std::condition_variable m_condvar;
    mutable std::mutex m_mutex;
};

namespace cppcapture {
    struct SenderThread::Pimpl {
        HTTPClientPtr httpClient;
        ChannelEncoderFn encoder;
        DataQueue queue;
        std::unique_ptr<std::thread> thread;
        std::atomic<bool> isRunning{ false };
    };

    SenderThread::SenderThread(HTTPClientPtr httpClient, ChannelEncoderFn encoder)
        : Channel("SenderThread"), m_pimpl(new Pimpl) {
        CPPCAPTURE_LOGDEBUG("creating new sender thread");
        m_pimpl->httpClient = std::move(httpClient);
        m_pimpl->encoder    = std::move(encoder);
        // start the thread
        m_pimpl->isRunning = true;
        m_pimpl->thread.reset(new std::thread{ [this] { run(); } });
    }

    SenderThread::~SenderThread() {
        CPPCAPTURE_LOGDEBUG("destroying sender thread");
        assert(m_pimpl->isRunning);
        if (m_pimpl->isRunning) {
            // it's the responsibility of the owner of the thread to flush the thread before destroying it
            // in order to not loose pending messages
            // here we don't want to block for too long that's why we don't do it
            stop();
        }
    }

    void SenderThread::stop() {
        CPPCAPTURE_LOGDEBUG("stopping sender thread");
        assert(m_pimpl->isRunning);
        if (m_pimpl->queue.size() > 0) {
            CPPCAPTURE_LOGERROR("stopping sender thread with a non empty queue: pending messages will be discarded");
        }

        // ask the thread to exit and refuse new request to send data
        m_pimpl->isRunning = false;
        m_pimpl->queue.close();
        // clearing the queue will speedup thread wakeup if it is blocked waiting for data
        m_pimpl->queue.clear();

        // wait for thread termination
        m_pimpl->thread->join();

        // do not delete m_pimpl as it still can be used to test isRunning from another function
        m_pimpl->thread.reset();
        m_pimpl->httpClient.reset();
    }

    void SenderThread::run() {
        assert(m_pimpl->isRunning);

        CPPCAPTURE_LOGDEBUG("entering sender thread main loop");
        while (m_pimpl->isRunning) {
            // wait for data to send
            const std::string str = m_pimpl->queue.readWithTimeout();
            if (str.empty()) {
                continue;
            }
            CPPCAPTURE_LOGDEBUG("got a new message to send from sender thread");

            // send only one data and return to sleep to avoid to block for too long the callers
            // if too many data is being pushed to the queue, they will be discarded by the callers
            // if we lost the connection and still have some pending messages, manage the size of the queue
            auto sleepTime = std::chrono::seconds{ 1 };

            // does not return until the message was sent, unless the operation was cancelled by the caller
            while (!m_pimpl->httpClient->sendRequest(str)) {
                if (!m_pimpl->isRunning) {
                    // we where asked to quit
                    CPPCAPTURE_LOGINFO("message was not sent (cancellation requested)");
                    break;
                }
                CPPCAPTURE_LOGERROR("message was not sent, pausing before retry");

                // TODO: use condition variable to wake up thread when asked to stop
                std::this_thread::sleep_for(sleepTime);

                // augment sleeping delay for next attempt until we reach 1 min
                if (sleepTime < std::chrono::seconds{ 30 }) {
                    sleepTime *= 2;
                }
            }
        }
        CPPCAPTURE_LOGINFO("exiting sender thread main loop");
    }

    void SenderThread::SendEvent(const cppcapture::Event & e) {
        CPPCAPTURE_LOGDEBUG("pushing a new event to the sender thread queue");
        if (!m_pimpl->isRunning) {
            CPPCAPTURE_LOGERROR("ignoring request to send message: sender thread is not running");
            return;
        }

        if (!m_pimpl->queue.push(m_pimpl->encoder(e))) {
            CPPCAPTURE_LOGERROR(
                "an event has been rejected by the sender thread: the queue might be full or the thread stopped");
        }
    }

    void SenderThread::Flush() {
        CPPCAPTURE_LOGDEBUG("flushing sender thread");
        if (!m_pimpl->isRunning) {
            return;
        }

        // do not hang for more than 5 sec
        for (size_t i = 0; i < 5 && m_pimpl->isRunning; i++) {
            size_t size = m_pimpl->queue.size();
            if (size == 0) {
                CPPCAPTURE_LOGDEBUG("flushing sender thread: done");
                return; // done!
            }
            CPPCAPTURE_LOGDEBUG("sending thread queue is not empty: flushing...");

            // processing a single message in the queue should not take longer than 1 sec
            // if it does we stop immediately because the connection is probably down,
            // or we may enqueue events faster that we can send them
            std::this_thread::sleep_for(s_readTimeout + std::chrono::seconds{ 1 });
            if (m_pimpl->queue.size() >= size) {
                CPPCAPTURE_LOGERROR("cancelling flush operation: message sending seems to be blocked");
                break;
            }
        }
        CPPCAPTURE_LOGERROR("could not complete flush operation: message queue is not being processed fast enough");
    }
} // namespace cppcapture
