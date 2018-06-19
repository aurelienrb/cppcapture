#pragma once

#include "raven/config.h"
#include "raven/event.h"

namespace raven {
    // configure default client
    ClientConfig & Configure();
    void Configure(const ClientConfig & config);

    // Client is used to send events to Sentry service
    class Client {
    public:
        // default main client
        static Client & Default();

        // configuration object
        ClientConfig & Config() { return m_config; }

        void Send(const Event & event);

        Client & operator<<(const Event & event) {
            Send(event);
            return *this;
        }

    private:
        ClientConfig m_config;
    };
} // namespace raven
