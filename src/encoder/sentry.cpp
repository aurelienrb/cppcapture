#include "sentry.h"
#include "stacktrace.h"
#include "encoder/json_encoder.h"
#include "network/http_socket.h"

#include <memory>

namespace cppcapture {
    std::string EncodeSentryEvent(const Event & event) {
        return event.ToJSON();
    }

    HTTPClientPtr NewSentryHTTPClient(int projectID, std::string token) {
        const std::string sentryAuth = "Sentry sentry_version=4, sentry_key=" + token;
        const std::string path       = "/api/" + std::to_string(projectID) + "/store/";

        // don't use make_unique to stay compatible with C++11
        std::unique_ptr<NativeSocketHTTPClient> httpClient{ new NativeSocketHTTPClient{ "sentry.io", 80, path } };
        httpClient->addHeader("X-Sentry-Auth", sentryAuth);
        httpClient->addHeader("Content-Type", "application/json");
        return HTTPClientPtr{ httpClient.release() }; // gcc 4.8 fails to cast std::unique_ptr
    }

    std::string EncodeSentryJSON(const std::vector<StackTraceEntry> & entries) {
        JsonEncoder json;
        json.beginBlock("sentry.Interfaces.Stacktrace");
        json.beginArray("frames");
        for (const auto & e : entries) {
            json.beginArrayElement();
            json.append("filename", e.fileName);
            json.append("function", e.functionName);
            json.append("lineno", e.lineNumber);
            json.append("module", e.moduleName);
            json.endArrayElement();
        }
        json.endArray();
        json.endBlock();
        return json.complete();

/*
        auto & array = json.beginArray("frames");
        for (const auto & e : entries) {
            array.newElement()
                .append("filename", e.fileName);
                .append("function", e.functionName);
                .append("lineno", e.lineNumber);
                .append("module", e.moduleName);
                .end();
        }
        array.end();
*/
    }
} // namespace cppcapture
