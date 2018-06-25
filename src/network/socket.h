#pragma once

#include <string>

namespace cppcapture {
    bool SendRawRequest(std::string hostName, int portNumber, const std::string & data, std::string & response);
} // namespace cppcapture
