#include "systemutils.h"

#include <windows.h>

namespace cppcapture {
    std::string newUUID() {
        UUID uuid;
        UuidCreate(&uuid);

        std::string result;
        RPC_CSTR szUuid = nullptr;
        if (::UuidToStringA(&uuid, &szUuid) == RPC_S_OK) {
            result = reinterpret_cast<const char *>(szUuid);
            ::RpcStringFreeA(&szUuid);
        }

        return result;
    }

    std::string getCurrentUserName() {
        return "aurelien";
    }

    std::string getLocalHostName() {
        return "PC";
    }
} // namespace cppcapture
