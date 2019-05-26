#include "cppcapture/anystring.h"

#include <doctest/doctest.h>

using cppcapture::AnyString;

TEST_CASE("Test AnyString") {
    CHECK(AnyString{ "abc" }.str == "abc");
    CHECK(AnyString{ std::string{ "abc" } }.str == "abc");
}

#ifdef CPPCAPTURE_WITH_QT
TEST_CASE("Test AnyString QString") {
    CHECK(AnyString{ QString{ "abc" } }.str == "abc");
    CHECK(AnyString{ QLatin1String{ "abc" } }.str == "abc");
}
#endif
