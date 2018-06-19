#pragma once

#include <functional>
#include <sstream>
#include <string>

#ifdef LOG_DEBUG_MESSAGES
#define LogDebug(...) raven::LogMessage("debug", __VA_ARGS__)
#else
#define LogDebug(...)
#endif

#define LogInfo(...) raven::LogMessage("info", __VA_ARGS__)
#define LogError(...) raven::LogMessage("error", __VA_ARGS__)

namespace raven {
    void LogMessage(const std::string & level, const std::string & msg);

    template <typename T>
    void MergeArgs(std::ostream & stream, T arg) {
        stream << arg;
    }

    template <typename T, typename... Args>
    void MergeArgs(std::ostream & stream, T arg, Args... others) {
        MergeArgs(stream, arg);
        MergeArgs(stream, others...);
    }

    template <typename... Args>
    void LogMessage(const std::string & level, const std::string & msg, Args... args) {
        std::ostringstream oss;
        MergeArgs(oss, msg, args...);
        LogMessage(level, oss.str());
    }

    class Guard {
    public:
        explicit Guard(std::function<void()> fn) : m_fn(fn) {}
        ~Guard() { m_fn(); }

    private:
        std::function<void()> m_fn;
    };
} // namespace raven
