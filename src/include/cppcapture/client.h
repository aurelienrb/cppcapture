#pragma once

#include "cppcapture/channel.h"
#include "cppcapture/event.h"

namespace cppcapture {
    // Client is used to send events to Sentry service
    class Client {
    public:
        static Client & Instance();

    public:
        // operator used to send a new event (via the macros)
        void operator+=(const Event & event);

        void SetChannel(ChannelPtr channel) {
            m_channel = std::move(channel);
        }

    private:
        Client();
        ~Client();

    private:
        ChannelPtr m_channel;
        // TODO: manage global session tags
        std::string m_serverName;  // example.com
        std::string m_release;     // 1.3.2
        std::string m_environment; // production
    };
} // namespace cppcapture

#define CaptureWarning(msg)                                                                                            \
    cppcapture::Client::Instance() += cppcapture::Event{                                                               \
        cppcapture::EventLevel::Warning, __func__                                                                      \
    }.WithLocation(__FILE__, __LINE__).WithMessage(msg)

#define CaptureError(msg)                                                                                              \
    cppcapture::Client::Instance() +=                                                                                  \
        cppcapture::Event{ cppcapture::EventLevel::Error, __func__ }.WithLocation(__FILE__, __LINE__).WithMessage(msg)

#define CaptureException(e)                                                                                            \
    cppcapture::Client::Instance() += cppcapture::Event{ cppcapture::EventLevel::Error, __func__ }                     \
                                          .WithLocation(__FILE__, __LINE__)                                            \
                                          .WithMessage(e.what())                                                       \
                                          .WithException(e)
