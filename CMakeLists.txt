cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# must be done before calling project()
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "CMAKE_BUILD_TYPE not set, using Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type, Debug or Release" FORCE)
endif()

project(cppcapture)

# options
option(CPPCAPTURE_BUILD_TESTS "Build the tests and examples coming with the library" OFF)
option(CPPCAPTURE_LOG_TRACES "Build to log maximum details (useful to debug socket exchanges)" OFF)

if (CMAKE_VERSION VERSION_LESS "3.1")
  # provide basic support for legacy systems such as Ubuntu 14.04 (gcc 4.8 / cmake 2.8)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else()
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON) # force C++11
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# adjust default compiler options
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /permissive-")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wunused -Werror")
  if(CMAKE_COMPILER_IS_GNUCXX)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug") 
      add_definitions(-D_FORTIFY_SOURCE=2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC)
    endif()
  endif()
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/third_party/third_parties.cmake)

add_subdirectory(src)

if (CPPCAPTURE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(examples)

  add_test(NAME cppcapture_test COMMAND cppcapture_test)
  add_test(NAME cppcapture_example COMMAND cppcapture_example)
endif()
