#pragma once

#include <string>

namespace raven {
    class ClientConfig {
    public:
        ClientConfig() = default;

        std::string hostname() const { return m_hostname; }
        ClientConfig & setUrlBase(std::string host) {
            m_hostname = host;
            return *this;
        }

        int port() const { return m_port; }
        ClientConfig & setPort(int p) {
            m_port = p;
            return *this;
        }

        int projectID() const { return m_projectID; }
        ClientConfig & setProjectID(int id) {
            m_projectID = id;
            return *this;
        }

        std::string token() const { return m_token; }
        ClientConfig & setToken(std::string tok) {
            m_token = tok;
            return *this;
        }

    private:
        std::string m_hostname = "sentry.io";
        int m_port             = 80; // http
        int m_projectID        = 0;
        std::string m_token;
        // session settings
        std::string m_serverName;  // example.com
        std::string m_release;     // 1.3.2
        std::string m_environment; // production
    };
} // namespace raven
