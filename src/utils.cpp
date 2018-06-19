#include "utils.h"
#include <iostream>

static void DefaultLogHandler(const std::string & level, const std::string & msg) {
    if (level == "error") {
        std::cerr << "(raven-cpp) " << level << ": " << msg << "\n";
    } else {
#if (defined(_WIN32) && defined(_DEBUG)) || !defined(NDEBUG)
        std::cout << "(raven-cpp) " << level << ": " << msg << "\n";
#endif
    }
}

// logger function to use (if any)
static std::function<void(std::string level, std::string msg)> s_logFn = &DefaultLogHandler;

namespace raven {
    void SetLibraryLogsHandler(std::function<void(std::string level, std::string msg)> fn) { s_logFn = fn; }

    void LogMessage(const std::string & level, const std::string & msg) {
        if (s_logFn) {
            s_logFn(level, msg);
        }
    }
} // namespace raven
