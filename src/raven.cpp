#include "encoder/sentry.h"
#include "raven/raven.h"
#include "sender_thread.h"

#include <iostream>
#include <mutex>

static void DefaultLogHandler(const std::string & level, const std::string & msg) {
    std::ostream * stream = nullptr;
    if (level == "error") {
        stream = &std::cerr;
    } else {
#ifdef _DEBUG
        stream = &std::cout;
#endif
    }

    if (stream) {
#ifdef _DEBUG
        // serialize messages in debug but don't interfer with client threads scheling in release
        static std::mutex s_mutex;
        std::lock_guard<std::mutex> lock{ s_mutex };
#endif

        *stream << "(raven-cpp) [";
        for (size_t i = level.length(); i < 5; i++) {
            *stream << ' ';
        }
        *stream << level << "] ";
        if (level == "debug") {
            *stream << "  ";
        }
        *stream << msg << "\n";
    }
}

// logger function to use (if any)
static std::function<void(std::string level, std::string msg)> s_logFn = &DefaultLogHandler;

namespace raven {
    Client & DefaultContext() {
        // static Client s_client;
        static std::unique_ptr<Client> s_client;
        if (!s_client) {
            s_client.reset(new Client{});
        }
        return *s_client;
    }

    void ConfigureSentry(int projectID, const std::string & token) {
        DefaultContext().SetChannel(SenderThread::StartNew(NewSentryHTTPClient(projectID, token), &EncodeSentryEvent));
    }

    void SetLibraryLogsHandler(std::function<void(std::string level, std::string msg)> fn) {
        s_logFn = fn;
    }

    void LogMessage(const std::string & level, const std::string & msg) {
        if (s_logFn) {
            s_logFn(level, msg);
        }
    }
} // namespace raven
