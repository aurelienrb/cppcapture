#pragma once

#include "cppcapture/client.h"

#define CPPCAPTURE_NAME "cppcapture"
#define CPPCAPTURE_VERSION "0.3"

#include <functional>
#include <string>

namespace cppcapture {
    // ConfigureSentry configures the given context to send events to Sentry
    // Can throw if the given configuration is invalid
    void ConfigureSentry(Client & client, int projectID, const std::string & token);

    // SetLibraryLogsHandler replaces the default log output handler that displays errors messages from the library
    // on std::cerr. A null handler can be given to disable log outputs.
    // Note: the handler is shared by all the clients / threads, and has to manage thread safety itself.
    void SetLibraryLogsHandler(std::function<void(std::string level, std::string msg)> fn);

    // DisableConsoleAlerts mute all messages that the library could write on the console.
    // By default console alerts are printed (on std::cerr) when things are not going well, such as when no DSN is
    // configured.
    inline void DisableConsoleAlerts() {
        SetLibraryLogsHandler(nullptr);
    }
} // namespace cppcapture
