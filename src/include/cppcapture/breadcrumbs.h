#pragma once

#include "cppcapture/anystring.h"
#include "cppcapture/eventlevel.h"

#include <mutex>
#include <unordered_map>
#include <vector>

namespace cppcapture {

    class JsonEncoder;

    // Utility class to help build messages for the Sentry Breadcrumb
    class BreadcrumbMessage {
    public:
        BreadcrumbMessage(EventLevel level, const char * functionName) : m_level(level) {
            if (functionName) {
                WithExtra("function", functionName);
            }
        }

        BreadcrumbMessage & WithMessage(AnyString msg) {
            m_message = std::move(msg.str);
            return *this;
        }

        BreadcrumbMessage & WithException(const std::exception & e) {
            return WithExtra("exception", e.what());
        }

        BreadcrumbMessage & WithExtra(AnyString key, AnyString value) {
            m_extra[std::move(key.str)] = std::move(value.str);
            return *this;
        }

        void PrintToLogger() const;
        void ToJSON(JsonEncoder &) const;

    private:
        EventLevel m_level = EventLevel::Error;
        std::string m_message;
        std::unordered_map<std::string, std::string> m_extra;
    };

    // Singleton class to manage the Sentry Breadcrumb
    class Breadcrumbs {
    public:
        static Breadcrumbs & Instance();

    public:
        // append a new message to the Breadcrumbs and display it to the logger (if configured)
        void operator+=(BreadcrumbMessage msg) {
            msg.PrintToLogger();
            std::lock_guard<std::mutex> lock{ m_mutex };
            m_messages.emplace_back(std::move(msg));
        }

        // clear all the existing messages in the Breadcrumbs
        Breadcrumbs & Reset() {
            std::lock_guard<std::mutex> lock{ m_mutex };
            m_messages.clear();
            return *this;
        }

        void ToJSON(JsonEncoder &) const;

    private:
        mutable std::mutex m_mutex;
        std::vector<BreadcrumbMessage> m_messages;
    };
}

#define BreadcrumbsResetHere()                                                                                         \
    cppcapture::Breadcrumbs::Instance().Reset() += cppcapture::BreadcrumbMessage {                                     \
        cppcapture::EventLevel::Info, __func__                                                                         \
    }

#define BreadcrumbsRecordHere()                                                                                        \
    cppcapture::Breadcrumbs::Instance() += cppcapture::BreadcrumbMessage {                                             \
        cppcapture::EventLevel::Info, __func__                                                                         \
    }

#define BreadcrumbsRecordInfo(msg)                                                                                     \
    cppcapture::Breadcrumbs::Instance() +=                                                                             \
        cppcapture::BreadcrumbMessage{ cppcapture::EventLevel::Info, __func__ }.WithMessage(msg)

#define BreadcrumbsRecordWarning(msg)                                                                                  \
    cppcapture::Breadcrumbs::Instance() +=                                                                             \
        cppcapture::BreadcrumbMessage{ cppcapture::EventLevel::Warning, __func__ }.WithMessage(msg)

#define BreadcrumbsRecordError(msg)                                                                                    \
    cppcapture::Breadcrumbs::Instance() +=                                                                             \
        cppcapture::BreadcrumbMessage{ cppcapture::EventLevel::Error, __func__ }.WithMessage(msg)

#define BreadcrumbsRecordException(e)                                                                                  \
    cppcapture::Breadcrumbs::Instance() +=                                                                             \
        cppcapture::BreadcrumbMessage{ cppcapture::EventLevel::Error, __func__ }.WithException(e)
