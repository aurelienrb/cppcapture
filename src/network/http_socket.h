#pragma once

#include "httpclient.h"

namespace raven {
    class NativeSocketHTTPClient : public HTTPClient {
    public:
        NativeSocketHTTPClient(std::string hostname, int port, std::string path);

        void addHeader(const std::string & key, const std::string & value);

    private:
        bool sendRequest(const std::string & body) override;

    private:
        std::string m_hostname;
        int m_port;
        std::string m_httpHeader;
    };
} // namespace raven
