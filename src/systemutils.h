#pragma once

#include "common.h"

namespace raven {
    namespace system {
        // generates a new UUID
        std::string newUUID();

        // returns the current user session name
        std::string getCurrentUserName();

        // returns the system host name
        std::string getLocalHostName();
    }
}
