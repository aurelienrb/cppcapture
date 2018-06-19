#include "http_encoder.h"
#include "network_socket.h"
#include "raven/client.h"
#include "utils.h"

#include <cassert>

namespace raven {
    ClientConfig & Configure() { return Client::Default().Config(); }

    void Configure(const ClientConfig & config) { Client::Default().Config() = config; }

    Client & Client::Default() {
        static Client m_client;
        return m_client;
    }

    void Client::Send(const Event & event) {
        // don't create a new event if not properly configured yet
        assert(!m_config.token().empty());
        assert(m_config.projectID() != 0);

        if (m_config.token().empty() || m_config.projectID() == 0) {
            LogError("not sending the event: configuration is missing");
            return;
        }

        // make sure we don't leak an exception
        try {
            std::string request = HTTPEncodePOSTRequest(m_config, event.ToJSON());
            SendRawRequest(m_config.hostname(), m_config.port(), std::move(request));
        } catch (const std::exception & e) {
            LogError("caught an exception while sending event: ", e.what());
        }
    }
} // namespace raven
