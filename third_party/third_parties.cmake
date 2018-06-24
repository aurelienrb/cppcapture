if (CMAKE_VERSION VERSION_LESS "3.0")
  # cmake 2.8 does not support INTERFACE targets
  file(WRITE empty.cpp "")
  add_library(catch empty.cpp)
  include_directories(${CMAKE_CURRENT_LIST_DIR}/catch/include)
else()
  add_library(catch INTERFACE)
  target_include_directories(catch INTERFACE ${CMAKE_CURRENT_LIST_DIR}/catch/include)
endif()
