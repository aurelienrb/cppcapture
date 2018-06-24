#pragma once

#include <string>

namespace raven {
    bool SendRawRequest(std::string hostName, int portNumber, const std::string & data, std::string & response);
} // namespace raven
