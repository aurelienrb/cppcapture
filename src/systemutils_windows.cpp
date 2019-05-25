#include "systemutils.h"

#include <iostream>
#include <vector>

#include <windows.h>
#pragma comment(lib, "rpcrt4.lib")

#define UNDNAME_COMPLETE 0x0000  // Enable full undecoration
#define UNDNAME_NAME_ONLY 0x1000 // Crack only the name for primary declaration. Does expand template parameters

// Internal (undocumented) VC++ CRT function to demangle symbol names.
extern "C" char * __unDName(char * outputString,
    const char * name,
    int maxStringLength,
    void * (*pAlloc)(size_t),
    void (*pFree)(void *),
    unsigned short disableFlags);

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

    std::string demangle(const char * name) {
        std::string result;

        // Although undocumented, __unDName is a better alternative than UnDecorateSymbolName which does not seem
        // to be updated to support the latest VC++ versions
        if (char * const pTmpUndName = __unDName(0, name, 0, malloc, free, UNDNAME_NAME_ONLY)) {
            result = pTmpUndName;
            free(pTmpUndName);
        } else {
            result = name;
        }
        return result;
    }
} // namespace cppcapture
