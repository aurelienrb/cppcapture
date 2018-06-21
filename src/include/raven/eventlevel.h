#pragma once

namespace raven {
    enum class EventLevel {
        Fatal,
        Error,
        Warning,
        Info,
        Debug,
    };

    inline const char * ToString(EventLevel level) {
        switch (level) {
        case EventLevel::Fatal:
            return "fatal";
        case EventLevel::Error:
            return "error";
        case EventLevel::Warning:
            return "warning";
        case EventLevel::Info:
            return "info";
        case EventLevel::Debug:
            return "debug";
        default:
            return "";
        }
    }
} // namespace raven
