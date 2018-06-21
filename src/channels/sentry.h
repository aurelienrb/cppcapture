#pragma once

#include "raven/channel.h"

namespace raven {
    class ChannelSentryHTTP : public Channel {
    public:
        ChannelSentryHTTP(int projectID, std::string token) : m_projectID(projectID), m_token(token) {}

    private:
        void SendEvent(const Event & event) override;

    private:
        std::string m_hostname = "sentry.io";
        int m_port             = 80; // http
        int m_projectID        = 0;
        std::string m_token;
    };
} // namespace raven
