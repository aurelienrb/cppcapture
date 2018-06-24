#pragma once

#define RAVEN_VERSION "0.2"

#include "raven/client.h"
#include "raven/config.h"
#include "raven/event.h"

#include <functional>
#include <string>

#define RavenCaptureWarning(msg)                                                                                       \
    raven::DefaultContext() << raven::Event{ raven::EventLevel::Warning }                                              \
                                   .WithFunctionLocation(__func__)                                                     \
                                   .WithFileLocation(__FILE__, __LINE__)                                               \
                                   .WithMessage(msg)

#define RavenCaptureError(msg)                                                                                         \
    raven::DefaultContext() << raven::Event{ raven::EventLevel::Error }                                                \
                                   .WithFunctionLocation(__func__)                                                     \
                                   .WithFileLocation(__FILE__, __LINE__)                                               \
                                   .WithMessage(msg)

#define RavenCaptureException(e)                                                                                       \
    raven::DefaultContext() << raven::Event{ raven::EventLevel::Error }                                                \
                                   .WithFunctionLocation(__func__)                                                     \
                                   .WithFileLocation(__FILE__, __LINE__)                                               \
                                   .WithMessage(e.what())                                                              \
                                   .WithException(e)

namespace raven {
    // DefaultClient returns the default context to be used for sending events
    Client & DefaultContext();

    // ConfigureSentry configures the default context to send events to Sentry
    // Can throw if the given configuration is invalid
    void ConfigureSentry(int projectID, const std::string & token);

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
} // namespace raven
