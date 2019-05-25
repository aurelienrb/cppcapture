#include "systemutils.h"
#include <exception>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace cppcapture;

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
    CHECK(makeExceptionType(ExceptionClass{}) == "ExceptionClass");
    CHECK(makeExceptionType(ExceptionStruct{}) == "ExceptionStruct");
}

#ifdef _MSC_VER // VC++
TEST_CASE("Test demangle_vc++17") {
    //CHECK(demangle(typeid(&ExceptionClass::what).raw_name()) == typeid(ExceptionClass).name());
    CHECK(demangle("?isSubstring@StringRef@Catch@@AEBA_NXZ") == "Catch::StringRef::isSubstring");
    CHECK(demangle(
              "??HCatch@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVStringRef@0@PEBD@Z") ==
          "Catch::operator+");
    CHECK(demangle("??1IConfig@Catch@@UEAA@XZ") == "Catch::IConfig::~IConfig");
    CHECK(demangle("?convert@?$StringMaker@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@X@Catch@@SA?"
                   "AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBV34@@Z") ==
          "Catch::StringMaker<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void>::convert");
    CHECK(demangle("?convert@?$StringMaker@V?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@std@@X@Catch@@"
                   "SA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBV?$basic_string@_WU?$char_"
                   "traits@_W@std@@V?$allocator@_W@2@@4@@Z") == "Catch::StringMaker<std::basic_string<wchar_t,std::"
                                                                "char_traits<wchar_t>,std::allocator<wchar_t> "
                                                                ">,void>::convert");
}
#endif
