#include "http_socket.h"
#include "network/socket.h"
#include "raven/raven.h"

#include <sstream>

namespace raven {
    NativeSocketHTTPClient::NativeSocketHTTPClient(std::string hostname, int port, std::string path)
        : m_hostname(hostname), m_port(port) {
        std::ostringstream oss;
        oss << "POST " << path << " HTTP/1.1\r\n";
        oss << "Host: " << hostname << "\r\n";
        oss << "User-Agent: raven/" << RAVEN_VERSION << "\r\n";

        // this is important for a one shot operation
        // otherwise the server won't close the socket once we read its response
        oss << "Connection: close\r\n"; // Connection: keep-alive

        m_httpHeader = oss.str();
    }

    void NativeSocketHTTPClient::addHeader(const std::string & key, const std::string & value) {
        m_httpHeader += key + ": " + value + "\r\n";
    }

    bool NativeSocketHTTPClient::sendRequest(const std::string & body) {
        std::string request;
        request.reserve(m_httpHeader.size() + 100 + body.size());

        request = m_httpHeader;
        request += "Content-Length: " + std::to_string(body.length()) + "\r\n";
        request += "\r\n";
        request += body;

        std::string response;
        return SendRawRequest(m_hostname, m_port, request, response);
    }
} // namespace raven
