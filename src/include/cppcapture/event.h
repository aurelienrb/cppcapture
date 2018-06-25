#pragma once

#include <string>
#include <unordered_map>

#include "cppcapture/eventlevel.h"

namespace cppcapture {
    class Event {
    public:
        explicit Event(EventLevel level) : m_level(level) {
        }
        Event(Event &&)      = default;
        Event(const Event &) = delete;

        Event & WithFunctionLocation(const char * functionName) {
            m_functionName = functionName;
            return *this;
        }

        // On Windows, path can include anti-slashes
        Event & WithFileLocation(const char * sourceFile, int lineNumber = 0) {
            m_sourceFile = sourceFile;
            m_lineNumber = lineNumber;
            return *this;
        }

        // Set the LoggerName info.
        // May be not very useful in a C++ context since we have source file location macros, but can make sense
        // if combined with a logging library such as LOG4CXX that has such a concept.
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

        Event & WithTag(const std::pair<std::string, std::string> & p) {
            return WithTag(p.first, p.second);
        }

        Event & WithExtra(const std::string & key, const std::string & value) {
            m_extra[key] = value;
            return *this;
        }

        Event & WithExtra(const std::pair<std::string, std::string> & p) {
            return WithExtra(p.first, p.second);
        }

        Event & WithException(const std::exception & e);

        Event & WithException(std::string type, std::string value) {
            m_exceptionType  = std::move(type);
            m_exceptionValue = std::move(value);
            return *this;
        }

        // Event & WithStacktrace(const std::string & msg);

        std::string ToJSON() const;

    private:
        EventLevel m_level          = EventLevel::Error;
        const char * m_functionName = nullptr;
        const char * m_sourceFile   = nullptr;
        int m_lineNumber            = 0;
        std::string m_message;
        std::string m_exceptionType;
        std::string m_exceptionValue;
        std::string m_loggerName;
        std::unordered_map<std::string, std::string> m_tags;
        std::unordered_map<std::string, std::string> m_extra;
    };
} // namespace cppcapture
