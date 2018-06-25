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
    LogError(std::string{ "failed to demangle VC++ type name: " } + name);
    return name;
}
#else
#include <cxxabi.h>

static std::string demangle(const std::type_info & type) {
    int status;
    char * demangled         = abi::__cxa_demangle(type.name(), NULL, NULL, &status);
    const std::string result = (status == 0) ? demangled : type.name();
    free(demangled);
    return result;
}
#endif

namespace cppcapture {
    std::string normalizePath(const char * path) {
        std::string result = (path != nullptr) ? path : "";
#ifdef _WIN32
        std::replace(result.begin(), result.end(), '\\', '/');
#endif
        return result;
    }

    std::string makeTimestamp() {
        time_t now;
        time(&now);

        struct tm utcTime;
        gmtime_r(&now, &utcTime);

        char buffer[32];
        strftime(buffer, sizeof buffer, "%FT%TZ", &utcTime);
        return buffer;
    }

    std::string makeExceptionType(const std::exception & e) {
        return demangle(typeid(e));
    }
} // namespace cppcapture
