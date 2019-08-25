#pragma once

#include <string>

namespace cppcapture {
    // generates a new UUID
    std::string newUUID();

    // current user session name
    std::string getCurrentUserName();

    // system host name
    std::string getLocalHostName();

    // source file name as it should be seen in Sentry
    std::string normalizeSourceFileName(const char * path);

    // current UTC timestamp in ISO 8601 (as required by Sentry)
    std::string makeTimestampISO8601();

    // type name of the given exception instance
    std::string makeExceptionType(const std::exception & e);
} // namespace cppcapture
