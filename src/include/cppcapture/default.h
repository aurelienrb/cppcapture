#pragma once

#include "cppcapture/client.h"
#include "cppcapture/event.h"

#define CaptureWarning(msg)                                                                                            \
    cppcapture::DefaultContext() << cppcapture::Event{ cppcapture::EventLevel::Warning }                               \
                                        .WithFunctionLocation(__func__)                                                \
                                        .WithFileLocation(__FILE__, __LINE__)                                          \
                                        .WithMessage(msg)

#define CaptureError(msg)                                                                                              \
    cppcapture::DefaultContext() << cppcapture::Event{ cppcapture::EventLevel::Error }                                 \
                                        .WithFunctionLocation(__func__)                                                \
                                        .WithFileLocation(__FILE__, __LINE__)                                          \
                                        .WithMessage(msg)

#define CaptureException(e)                                                                                            \
    cppcapture::DefaultContext() << cppcapture::Event{ cppcapture::EventLevel::Error }                                 \
                                        .WithFunctionLocation(__func__)                                                \
                                        .WithFileLocation(__FILE__, __LINE__)                                          \
                                        .WithMessage(e.what())                                                         \
                                        .WithException(e)

namespace cppcapture {
    // DefaultClient returns the default context to be used for sending events
    inline Client & DefaultContext() {
        static Client s_client;
        return s_client;
    }
} // namespace cppcapture
