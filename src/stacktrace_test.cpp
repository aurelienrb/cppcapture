#include "stacktrace.h"
#include <catch.hpp>

#include <algorithm>
#include <functional>

using namespace cppcapture;

static std::vector<StackTraceEntry> entries;

#ifdef _MSC_VER // VC++
// disable optimization to get a stacktrace that reflects the code we use
#pragma optimize("", off)

class StacktraceTestClass {
public:
    explicit StacktraceTestClass(std::function<int()> f) {
        f();
    }
};

struct StacktraceTestStruct {
    void callFromClass(int x) {
        StacktraceTestClass{ [x]() {
            entries = captureCurrentThreadStackTrace();
            return x;
        } };
    }
};

static void StacktraceTestF1() {
    StacktraceTestStruct s;
    s.callFromClass(10);
}

static std::string StacktraceTestF2() {
    StacktraceTestF1();
    return "";
}

TEST_CASE("Test stacktrace_windows") {
    entries.clear();
    StacktraceTestF2();
    REQUIRE(entries.size() > 0);

    // count how many function names start with "StacktraceTest"
    const auto count = std::count_if(entries.begin(), entries.end(), [](const StackTraceEntry & e) {
        const std::string pattern{ "StacktraceTest" };
        return (e.functionName.substr(0, pattern.length()) == pattern);
    });
    CHECK(count == 4);
}
#endif
