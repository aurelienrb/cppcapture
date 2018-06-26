#include "stacktrace.h"
#include "systemutils.h"
#include <StackWalker/StackWalker.h>

class CaptureCallStack : public StackWalker {
public:
    explicit CaptureCallStack(std::vector<cppcapture::StackTraceEntry> & entries) : m_entries(entries) {
    }

private:
    // utility function make sure to correctly read the raw string
    static std::string to_safe_string(const char * str) {
        if (str[0] == '\0') {
            return "";
        } else {
            const size_t length = strnlen_s(str, STACKWALK_MAX_NAMELEN);
            return std::string{ str, length };
        }
    }

    void OnCallstackEntry(CallstackEntryType eType, CallstackEntry & entry) override {
        if ((eType != lastEntry) && (entry.offset != 0)) {
            cppcapture::StackTraceEntry result;
            result.fileName     = cppcapture::normalizePath(to_safe_string(entry.lineFileName));
            result.functionName = to_safe_string(entry.name);
            result.lineNumber   = entry.lineNumber;
            result.moduleName   = to_safe_string(entry.moduleName);
            m_entries.push_back(result);
        }
    }

    std::vector<cppcapture::StackTraceEntry> & m_entries;
};

namespace cppcapture {
    std::vector<StackTraceEntry> captureCurrentThreadStackTrace() {
        std::vector<StackTraceEntry> entries;
        entries.reserve(20);

        CaptureCallStack cc{ entries };
        cc.ShowCallstack();

        return entries;
    }
} // namespace cppcapture
