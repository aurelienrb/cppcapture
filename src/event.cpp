#include "json_encoder.h"
#include "raven/raven.h"
#include "utils.h"

#include <algorithm>
#include <cstring>
#include <time.h>

#ifdef _WIN32
static errno_t gmtime_r(const time_t * timep, struct tm * result) {
    return gmtime_s(result, timep);
}
#endif

static std::string makeTimestamp() {
    time_t now;
    time(&now);

    struct tm utcTime;
    gmtime_r(&now, &utcTime);

    char buffer[32];
    strftime(buffer, sizeof buffer, "%FT%TZ", &utcTime);
    return buffer;
}

static std::string makeExceptionType(const std::exception & e) {
#ifdef _MSC_VER
#ifndef _CPPRTTI
// we need RTTI for makeExceptionType()
#error "RTTI support must be enabled"
#endif
#endif
    const char * name = typeid(e).name();
    // we expect something like "class std::runtime_exception"
    if (strncmp(name, "class ", 6) == 0) {
        return name + 6;
    } else if (strncmp(name, "struct ", 7) == 0) {
        return name + 7;
    }
    return std::string{ name };
}

static std::string makeCulprit(const char * sourceFile, int lineNumber) {
    std::string location = sourceFile;
#ifdef _WIN32
    std::replace(location.begin(), location.end(), '\\', '/');
#endif
    if (lineNumber > 0) {
        location += ":" + std::to_string(lineNumber);
    }
    return location;
}

namespace raven {
    Event & Event::WithException(const std::exception & e) { return WithException(makeExceptionType(e), e.what()); }

    std::string Event::ToJSON() const {
        JsonEncoder json;

        // event_id is optional
        // json.append(json, "event_id", "32cebd5d-1542-4703-a5a3-be5eaa90af81");

        // A string representing the platform the SDK is submitting from. This will be used by the Sentry interface
        // to customize various components in the interface.
        // "c++" is not a supported olatform so for now we use c
        json.append("platform", "c");

        // Information about the SDK sending the event.
        json.beginBlock("sdk").append("name", "raven-cpp").append("version", RAVEN_VERSION).endBlock();

        /*
        if (!s_values.ServerName.empty()) {
        json.append("server_name", s_values.ServerName);
        }
        if (!s_values.ReleaseVersion.empty()) {
        json.append("release", s_values.ReleaseVersion);
        }
        if (!s_values.Environment.empty()) {
        json.append("environment", s_values.Environment);
        }
        */

        // The timestamp should be in ISO 8601 format, without a timezone (UTC)
        json.append("timestamp", makeTimestamp());

        // The name of the logger which created the record
        json.append("logger", m_loggerName);

        if (m_sourceFile != nullptr) {
            json.append("culprit", makeCulprit(m_sourceFile, m_lineNumber));
        }

        json.append("level", ToString(m_level));
        json.append("tags", m_tags);
        json.append("extra", m_extra);

        if (!m_exceptionType.empty()) {
            json.beginBlock("sentry.interfaces.Exception")
                .append("type", m_exceptionType)
                .append("value", m_exceptionValue)
                .endBlock();
        }

        if (!m_message.empty()) {
            json.beginBlock("sentry.interfaces.Message")
                .append("message", m_message)
                //.append("params", R"(["Param1"])")
                .endBlock();
        }

        return json.complete();
    }
} // namespace raven
