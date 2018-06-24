#pragma once

#include "raven/channel.h"
#include "raven/event.h"

namespace raven {
    // Client is used to send events to Sentry service
    class Client {
    public:
        Client();
        ~Client();

        // prevent copy
        Client(const Client &) = delete;
        void operator=(const Client &) = delete;

        void Send(const Event & event);

        Client & operator<<(const Event & event) {
            Send(event);
            return *this;
        }

        void SetChannel(ChannelPtr channel) { m_channel = std::move(channel); }

    private:
        ChannelPtr m_channel;
        // TODO: manage global session tags
        std::string m_serverName;  // example.com
        std::string m_release;     // 1.3.2
        std::string m_environment; // production
    };
} // namespace raven
