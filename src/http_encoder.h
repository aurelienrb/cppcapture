#pragma once

#include "raven/config.h"

#include <sstream>
#include <string>

namespace raven {
    inline std::string HTTPEncodePOSTRequest(const ClientConfig & config, const std::string & body) {
        std::string sentryAuth = "Sentry sentry_version=4, sentry_key=" + config.token();
        std::string path       = "/api/" + std::to_string(config.projectID()) + "/store/";

        std::ostringstream oss;
        oss << "POST " << path << " HTTP/1.1\r\n";
        oss << "Host: " << config.hostname() << "\r\n";
        oss << "User-Agent: curl/7.52.1\r\n";
        oss << "Connection: close\r\n"; // this is important
        oss << "X-Sentry-Auth: " << sentryAuth << "\r\n";
        oss << "Content-Type: application/json\r\n";
        oss << "Content-Length: " << body.length() << "\r\n";
        oss << "\r\n";
        oss << body;

        return oss.str();
    }
} // namespace raven
