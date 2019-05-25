#pragma once

#include <string>

namespace cppcapture {
    // generates a new UUID
    std::string newUUID();

    // returns the current user session name
    std::string getCurrentUserName();

    // returns the system host name
    std::string getLocalHostName();

    std::string normalizePath(const char * path);

    std::string makeTimestampISO8601();

    std::string makeExceptionType(const std::exception & e);
} // namespace cppcapture
