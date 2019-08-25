#include "json_encoder.h"

#include <doctest/doctest.h>

TEST_CASE("Test json_encoder") {
    static const char * expectedJson = R"({
 "block1": {
  "block2": {
   "e1": "1",
   "e2": 2
  },
  "block3": {
   "array1": [
    {
     "e3": "3"
    }
   ],
   "e4": -4,
   "array2": [
    {
     "e5": "5",
     "nestedArray": [
      {
       "nestedBlock": {
        "e7": "7"
       }
      }
     ],
     "e6": 6
    }
   ]
  }
 }
})";

    cppcapture::JsonEncoder json;
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
    json.beginArray("nestedArray");
    json.beginArrayElement();
    json.beginBlock("nestedBlock");
    json.append("e7", "7");
    json.endBlock();
    json.endArrayElement();
    json.endArray();
    json.append("e6", 6);
    json.endArrayElement();
    json.endArray();
    json.endBlock();

    json.endBlock();

    const auto text = json.complete();
    CHECK(text == expectedJson);
}
