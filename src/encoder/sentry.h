#pragma once

#include "raven/event.h"
#include "network/httpclient.h"

#include <string>

namespace raven {
    std::string EncodeSentryEvent(const Event & event);

    HTTPClientPtr NewSentryHTTPClient(int projectID, std::string token);
} // namespace raven

