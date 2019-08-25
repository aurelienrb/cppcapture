#pragma once

#include "cppcapture/anystring.h"
#include "cppcapture/eventlevel.h"

#include <unordered_map>

namespace cppcapture {
    class Event {
    public:
        Event(EventLevel level, const char * functionName) : m_level(level), m_functionName(functionName) {
        }
        Event(Event &&)      = default;
        Event(const Event &) = delete;

        // On Windows, path can include anti-slashes
        Event & WithLocation(const char * sourceFile, int lineNumber) {
            m_sourceFile = sourceFile;
            m_lineNumber = lineNumber;
            return *this;
        }

        // Set the LoggerName info.
        // May be not very useful in a C++ context since we have source file location macros, but can make sense
        // if combined with a logging library such as LOG4CXX that has such a concept.
        Event & WithLoggerName(AnyString loggerName) {
            m_loggerName = std::move(loggerName.str);
            return *this;
        }

        Event & WithMessage(AnyString msg) {
            m_message = std::move(msg.str);
            return *this;
        }

        Event & WithTag(const AnyString & key, const AnyString & value) {
            m_tags[key.str] = value.str;
            return *this;
        }

        Event & WithTag(const std::pair<AnyString, AnyString> & p) {
            return WithTag(p.first, p.second);
        }

        Event & WithExtra(const AnyString & key, const AnyString & value) {
            m_extra[key.str] = value.str;
            return *this;
        }

        Event & WithException(const std::exception & e);

        Event & WithException(AnyString type, AnyString value) {
            m_exceptionType  = std::move(type.str);
            m_exceptionValue = std::move(value.str);
            return *this;
        }

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
