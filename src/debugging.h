#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

// use Qt logging facilities if available
#ifdef CPPCAPTURE_WITH_QT
#include <QDebug>
#define CPPCAPTURE_LOGDEBUG(...)                                                                                       \
    qDebug() << "(cppcapture)" << QString::fromStdString(::cppcapture::LogConcat(__VA_ARGS__))
#define CPPCAPTURE_LOGINFO(...)                                                                                        \
    qInfo() << "(cppcapture)" << QString::fromStdString(::cppcapture::LogConcat(__VA_ARGS__))
#define CPPCAPTURE_LOGERROR(...)                                                                                       \
    qCritical() << "(cppcapture)" << QString::fromStdString(::cppcapture::LogConcat(__VA_ARGS__))
#endif

// define default logging macros in a flexible way
#if !defined(CPPCAPTURE_NODEBUG) && !defined(CPPCAPTURE_LOGDEBUG)
#define CPPCAPTURE_LOGDEBUG(...) ::cppcapture::LogMessage("DEBUG", __VA_ARGS__)
#else
#ifdef CPPCAPTURE_LOGDEBUG
#undef CPPCAPTURE_LOGDEBUG
#endif
#define CPPCAPTURE_LOGDEBUG(...)
#endif

#ifndef CPPCAPTURE_LOGINFO
#define CPPCAPTURE_LOGINFO(...) ::cppcapture::LogMessage("INFO", __VA_ARGS__)
#endif

#ifndef CPPCAPTURE_LOGERROR
#define CPPCAPTURE_LOGERROR(...) ::cppcapture::LogMessage("ERROR", __VA_ARGS__)
#endif

namespace cppcapture {
    template <typename T>
    void MergeArgs(std::ostream & stream, T arg) {
        stream << arg;
    }

    template <typename T, typename... Args>
    void MergeArgs(std::ostream & stream, T arg, Args... others) {
        MergeArgs(stream, arg);
        MergeArgs(stream, others...);
    }

    inline std::string LogConcat(std::string msg) {
        return msg;
    }

    template <typename... Args>
    std::string LogConcat(const std::string & msg, Args... args) {
        std::ostringstream oss;
        MergeArgs(oss, msg, args...);
        return oss.str();
    }

    void LogMessage(const std::string & level, const std::string & msg);

    template <typename... Args>
    void LogMessage(const std::string & level, const std::string & msg, Args... args) {
        std::ostringstream oss;
        MergeArgs(oss, msg, args...);
        LogMessage(level, oss.str());
    }
} // namespace cppcapture
