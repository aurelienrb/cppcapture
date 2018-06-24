#pragma once

#include "network/httpclient.h"
#include "raven/channel.h"

#include <functional>
#include <vector>

namespace raven {
    using ChannelEncoderFn = std::function<std::string(const raven::Event &)>;

    class SenderThread : public raven::Channel {
    public:
        static std::shared_ptr<SenderThread> StartNew(HTTPClientPtr httpClient, ChannelEncoderFn encoder) {
            return std::shared_ptr<SenderThread>{ new SenderThread{ std::move(httpClient), std::move(encoder) } };
        }

        ~SenderThread();
        // disable copy
        SenderThread(const SenderThread &) = delete;
        void operator=(const SenderThread &) = delete;

    private:
        SenderThread(HTTPClientPtr httpClient, ChannelEncoderFn encoder);
        // stop as fast as possible (discard pending results)
        void stop();

    protected:
        void SendEvent(const raven::Event & e) override;
        void Flush() override;

    private:
        void run();

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> m_pimpl;
    };
} // namespace raven
