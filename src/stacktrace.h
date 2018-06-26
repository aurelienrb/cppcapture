#pragma once

#include <string>
#include <vector>

namespace cppcapture {
    struct StackTraceEntry {
        std::string functionName;
        std::string fileName;
        size_t lineNumber;
        std::string moduleName;
    };

    std::vector<StackTraceEntry> captureCurrentThreadStackTrace();
} // namespace cppcapture
