#pragma once

#include <string>

namespace raven {
    void SendRawRequest(std::string hostName, int portNumber, const std::string & data);
}
