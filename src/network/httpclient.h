#pragma once

#include <memory>
#include <string>

namespace cppcapture {
    using HTTPClientPtr = std::unique_ptr<class HTTPClient>;

    class HTTPClient {
    public:
        // The instance is responsible of closing down any active connection when it is destroyed.
        virtual ~HTTPClient() = default;

        // Tries to send a request to the server. Only the encoded body is passed, the implementation is
        // responsible of creating the proper HTTP headers and so on.
        // Must return fairly quickly and not try on its own to resend on failure.
        // For example, with a keep alive socket session, if when sending we realize the connexion was lost,
        // it can update its state to track the loss of connection but not reconnect immediately. Only when
        // it is called again it can try to connect before sending the packet.
        // It's up to the implementation to decide if it established the connection immediately when created
        // or to defer it to the first call to sendRequest().
        virtual bool sendRequest(const std::string & body) = 0;
    };
} // namespace cppcapture
