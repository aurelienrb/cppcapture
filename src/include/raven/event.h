#pragma once

#include <string>
#include <unordered_map>

namespace raven {
    enum class Level {
        Fatal,
        Error,
        Warning,
        Info,
        Debug,
    };

    inline const char * ToString(Level level) {
        switch (level) {
        case Level::Fatal:
            return "fatal";
        case Level::Error:
            return "error";
        case Level::Warning:
            return "warning";
        case Level::Info:
            return "info";
        case Level::Debug:
            return "debug";
        default:
            return "";
        }
    }

    class Event {
    public:
        explicit Event(Level level) : m_level(level) {}
        Event(Event &&)      = default;
        Event(const Event &) = delete;

        Event & WithFileLocation(const char * sourceFile, int lineNumber = 0) {
            m_sourceFile = sourceFile;
            m_lineNumber = lineNumber;
            return *this;
        }

        // not very useful in C++ unless we use something like log4cxx?
        Event & WithLoggerName(std::string loggerName) {
            m_loggerName = std::move(loggerName);
            return *this;
        }

        Event & WithMessage(std::string msg) {
            m_message = std::move(msg);
            return *this;
        }

        Event & WithTag(const std::string & key, const std::string & value) {
            m_tags[key] = value;
            return *this;
        }

        Event & WithTag(const std::pair<std::string, std::string> & p) { return WithTag(p.first, p.second); }

        Event & WithExtra(const std::string & key, const std::string & value) {
            m_extra[key] = value;
            return *this;
        }

        Event & WithExtra(const std::pair<std::string, std::string> & p) { return WithExtra(p.first, p.second); }

        Event & WithException(const std::exception & e);

        Event & WithException(std::string type, std::string value) {
            m_exceptionType  = std::move(type);
            m_exceptionValue = std::move(value);
            return *this;
        }

        // Event & WithStacktrace(const std::string & msg);

        std::string ToJSON() const;

    private:
        Level m_level;
        const char * m_sourceFile = nullptr;
        int m_lineNumber          = 0;
        std::string m_message;
        std::string m_exceptionType;
        std::string m_exceptionValue;
        std::string m_loggerName;
        std::unordered_map<std::string, std::string> m_tags;
        std::unordered_map<std::string, std::string> m_extra;
    };
} // namespace raven
