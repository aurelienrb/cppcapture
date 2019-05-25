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

    inline std::string normalizePath(const std::string & path) {
        return normalizePath(path.c_str());
    }

    std::string makeTimestamp();

    std::string makeExceptionType(const std::exception & e);

    std::string demangle(const char * name);
} // namespace cppcapture
