cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(cppcapture VERSION 0.8)

# options
option(BUILD_TESTING "Build unit tests and examples" OFF)
option(CPPCAPTURE_WITH_QT "Enable better integration with Qt (adds a dependency to QtCore)" OFF)
option(CPPCAPTURE_NODEBUG "Disable debug messages even in debug builds" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # force C++11
set(CMAKE_CXX_EXTENSIONS OFF)

# detect Qt if required
if (CPPCAPTURE_WITH_QT)
    find_package(Qt5 COMPONENTS Core REQUIRED)
endif()

if (BUILD_TESTING)
    enable_testing()
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

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(examples)
