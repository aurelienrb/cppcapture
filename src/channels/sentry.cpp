#include "sentry.h"
#include "socket.h"

#include <sstream>

static std::string EncodeSentryPOSTRequest(
    const std::string & hostname, int projectID, const std::string & token, const std::string & body) {
    const std::string sentryAuth = "Sentry sentry_version=4, sentry_key=" + token;
    const std::string path       = "/api/" + std::to_string(projectID) + "/store/";

    std::ostringstream oss;
    oss << "POST " << path << " HTTP/1.1\r\n";
    oss << "Host: " << hostname << "\r\n";
    oss << "User-Agent: curl/7.52.1\r\n";
    // this is important for a one shot operation, otherwise the server won't close the socket once we read its response
    oss << "Connection: close\r\n";
    oss << "X-Sentry-Auth: " << sentryAuth << "\r\n";
    oss << "Content-Type: application/json\r\n";
    oss << "Content-Length: " << body.length() << "\r\n";
    oss << "\r\n";
    oss << body;

    return oss.str();
}

namespace raven {
    void ChannelSentryHTTP::SendEvent(const Event & event) {
        const std::string request = EncodeSentryPOSTRequest(m_hostname, m_projectID, m_token, event.ToJSON());
        SendRawRequest(m_hostname, m_port, std::move(request));
    }
} // namespace raven
