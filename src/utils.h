#pragma once

#include <functional>
#include <sstream>
#include <string>

#if !defined(_WIN32) && !defined(NDEBUG)
#define _DEBUG
#endif

#define LogInfo(...) raven::LogMessage("info", __VA_ARGS__)
#define LogError(...) raven::LogMessage("error", __VA_ARGS__)

#if defined(_DEBUG) || defined(LOG_TRACE_MESSAGES)
#define LogDebug(...) raven::LogMessage("debug", __VA_ARGS__)
#else
#define LogDebug(...)
#endif

#ifdef LOG_TRACE_MESSAGES
#define LogTrace(...) raven::LogMessage("trace", __VA_ARGS__)
#else
#define LogTrace(...)
#endif

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
} // namespace raven
