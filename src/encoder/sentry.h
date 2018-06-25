#pragma once

#include "cppcapture/event.h"
#include "network/httpclient.h"

#include <string>

namespace cppcapture {
    std::string EncodeSentryEvent(const Event & event);

    HTTPClientPtr NewSentryHTTPClient(int projectID, std::string token);
} // namespace cppcapture
