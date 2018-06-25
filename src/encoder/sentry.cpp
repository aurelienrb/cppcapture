#include "sentry.h"
#include "network/http_socket.h"

#include <memory>

namespace cppcapture {
    std::string EncodeSentryEvent(const Event & event) {
        return event.ToJSON();
    }

    HTTPClientPtr NewSentryHTTPClient(int projectID, std::string token) {
        const std::string sentryAuth = "Sentry sentry_version=4, sentry_key=" + token;
        const std::string path       = "/api/" + std::to_string(projectID) + "/store/";

        // don't use make_unique to stay compatible with C++11
        std::unique_ptr<NativeSocketHTTPClient> httpClient{ new NativeSocketHTTPClient{ "sentry.io", 80, path } };
        httpClient->addHeader("X-Sentry-Auth", sentryAuth);
        httpClient->addHeader("Content-Type", "application/json");
        return HTTPClientPtr{ httpClient.release() }; // gcc 4.8 fails to cast std::unique_ptr
    }
} // namespace cppcapture
