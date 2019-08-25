#include "systemutils.h"
#include <exception>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

class ExceptionClass : public std::runtime_error {
public:
    ExceptionClass() : runtime_error("ExceptionClass") {
    }
};

struct ExceptionStruct : public std::logic_error {
public:
    ExceptionStruct() : logic_error("ExceptionStruct") {
    }
};

using namespace cppcapture;

TEST_CASE("Test makeExceptionType") {
    CHECK(makeExceptionType(ExceptionClass{}) == "ExceptionClass");
    CHECK(makeExceptionType(ExceptionStruct{}) == "ExceptionStruct");
}

TEST_CASE("Test normalizeSourceFileName") {
    CHECK(normalizeSourceFileName(nullptr) == "");
    CHECK(normalizeSourceFileName("") == "");
    CHECK(normalizeSourceFileName("/") == "/");
    CHECK(normalizeSourceFileName("file.cpp") == "file.cpp");
    CHECK(normalizeSourceFileName("parent/file.cpp") == "parent/file.cpp");
    CHECK(normalizeSourceFileName("parent\\file.cpp") == "parent/file.cpp");
    CHECK(normalizeSourceFileName("/parent/file.cpp") == "parent/file.cpp");
    CHECK(normalizeSourceFileName("\\parent\\file.cpp") == "parent/file.cpp");
    CHECK(normalizeSourceFileName("root/parent/file.cpp") == "parent/file.cpp");
    CHECK(normalizeSourceFileName("C:\\parent\\file.cpp") == "parent/file.cpp");
    CHECK(normalizeSourceFileName("root/path/parent/file.cpp") == "parent/file.cpp");
    CHECK(normalizeSourceFileName("C:\\path\\parent\\file.cpp") == "parent/file.cpp");
}
