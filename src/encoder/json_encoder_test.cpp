#include "json_encoder.h"
#include <catch.hpp>

TEST_CASE("Test json_encoder") {
    JsonEncoder json;
    json.beginBlock("block1");

    json.beginBlock("block2");
    json.append("e1", "1");
    json.append("e2", 2);
    json.endBlock();

    json.beginBlock("block3");
    json.beginArray("array1");
    json.beginArrayElement();
    json.append("e3", "3");
    json.endArrayElement();
    json.endArray();
    json.append("e4", -4);
    json.beginArray("array2");
    json.beginArrayElement();
    json.append("e5", "5");
    json.append("e6", 6);
    json.endArrayElement();
    json.endArray();
    json.endBlock();

    json.endBlock();

    const auto text = json.complete();

/* hello
*/ #if 0
    abc ca va bien ?
#endif /* OK
*/
}
