#include "cppcapture/configure.h"
#include "encoder/sentry.h"
#include "sender_thread.h"

#include <iomanip>
#include <iostream>
#include <mutex>

static void DefaultLogHandler(const std::string & level, const std::string & msg) {
    std::ostream * stream = nullptr;
    if (level == "error" || level == "ERROR") {
        stream = &std::cerr;
    } else {
        stream = &std::cout;
    }

    if (stream) {
        static std::mutex s_mutex;
        std::lock_guard<std::mutex> lock{ s_mutex };

        *stream << "[" << std::setw(5) << level << "] (cppcapture) " << msg << std::endl;
    }
}

// logger function to use (if any)
static std::function<void(std::string level, std::string msg)> s_logFn = &DefaultLogHandler;

namespace cppcapture {
    void ConfigureSentry(Client & client, int projectID, const std::string & token) {
        client.SetChannel(SenderThread::StartNew(NewSentryHTTPClient(projectID, token), &EncodeSentryEvent));
    }

    void SetLibraryLogsHandler(std::function<void(std::string level, std::string msg)> fn) {
        s_logFn = fn;
    }

    void LogMessage(const std::string & level, const std::string & msg) {
        if (s_logFn) {
            s_logFn(level, msg);
        }
    }
} // namespace cppcapture
