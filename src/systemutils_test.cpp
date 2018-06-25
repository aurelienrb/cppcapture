#include "systemutils.h"
#include <exception>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

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

TEST_CASE("Test makeExceptionType") {
    CHECK(cppcapture::makeExceptionType(ExceptionClass{}) == "ExceptionClass");
    CHECK(cppcapture::makeExceptionType(ExceptionStruct{}) == "ExceptionStruct");
}
