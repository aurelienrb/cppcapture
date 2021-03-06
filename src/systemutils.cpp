#include "systemutils.h"
#include "debugging.h"

#include <algorithm>
#include <cstring>
#include <ctime>

#ifdef _MSC_VER // VC++
// we need RTTI for makeExceptionType()
#ifndef _CPPRTTI
#error "RTTI support must be enabled"
#endif

static errno_t gmtime_r(const time_t * timep, struct tm * result) {
    return gmtime_s(result, timep);
}

static std::string demangle(const std::type_info & type) {
    const char * name = type.name();
    // we expect something like "class std::runtime_exception"
    if (strncmp(name, "class ", 6) == 0) {
        return name + 6;
    } else if (strncmp(name, "struct ", 7) == 0) {
        return name + 7;
    }
    CPPCAPTURE_LOGERROR("failed to demangle VC++ type name: ", name);
    return name;
}
#else
#include <cxxabi.h>

static std::string demangle(const std::type_info & type) {
    int status;
    char * demangled         = abi::__cxa_demangle(type.name(), nullptr, nullptr, &status);
    const std::string result = (status == 0) ? demangled : type.name();
    free(demangled);
    return result;
}
#endif

namespace cppcapture {
    std::string normalizeSourceFileName(const char * path) {
        // make sure to have normal slashes '/'
        std::string result = (path != nullptr) ? path : "";
        std::replace(result.begin(), result.end(), '\\', '/');
        assert(result.find("/../") == std::string::npos); // given path should be absolute

        // the full file path is too long to be displayed in Sentry: keep only its name + parent dir
        auto pos = result.find_last_of('/');
        if (pos != std::string::npos) {
            // we found the file name: limit the path to its parent dir if present
            if (pos > 0) {
                pos = result.find_last_of('/', pos - 1);
                if (pos != std::string::npos) {
                    result = result.substr(pos + 1);
                }
            }
        }

        return result;
    }

    std::string makeTimestampISO8601() {
        time_t now;
        time(&now);

        struct tm utcTime;
        gmtime_r(&now, &utcTime);

        char buffer[32];
        strftime(buffer, sizeof buffer, "%Y-%m-%dT%H:%M:%SZ", &utcTime);
        return buffer;
    }

    std::string makeExceptionType(const std::exception & e) {
        return demangle(typeid(e));
    }
} // namespace cppcapture
