set(cppcapture_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(cppcapture_SRC
    ${cppcapture_INC_DIR}/cppcapture/channel.h
    ${cppcapture_INC_DIR}/cppcapture/client.h
    ${cppcapture_INC_DIR}/cppcapture/configure.h
    ${cppcapture_INC_DIR}/cppcapture/default.h
    ${cppcapture_INC_DIR}/cppcapture/eventlevel.h
    ${cppcapture_INC_DIR}/cppcapture/event.h
    encoder/json_encoder.h
    encoder/sentry.cpp
    encoder/sentry.h
    network/httpclient.h
    network/http_socket.cpp
    network/http_socket.h
    network/socket.cpp
    network/socket.h
    channel.cpp
    client.cpp
    configure.cpp
    debugging.h
    event.cpp
    sender_thread.cpp
    sender_thread.h
    systemutils.h
    systemutils.cpp
)

if (WIN32)
    list(APPEND cppcapture_SRC systemutils_windows.cpp)
else()
    list(APPEND cppcapture_SRC systemutils_posix.cpp)
endif()

add_library(cppcapture ${cppcapture_SRC})
target_include_directories(cppcapture PUBLIC ${cppcapture_INC_DIR})
target_include_directories(cppcapture PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (CPPCAPTURE_NODEBUG OR NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(cppcapture PRIVATE -DCPPCAPTURE_NODEBUG)
endif()

if (WIN32)
    target_link_libraries(cppcapture PRIVATE ws2_32.lib)
else()
    target_link_libraries(cppcapture PRIVATE pthread)
endif()

if (CPPCAPTURE_WITH_QT)
    target_compile_definitions(cppcapture PUBLIC -DCPPCAPTURE_WITH_QT)
    target_link_libraries(cppcapture PUBLIC Qt5::Core)
endif()

if (BUILD_TESTING)
    add_executable(cppcapture_test systemutils_test.cpp)
    target_link_libraries(cppcapture_test cppcapture doctest)
    add_test(NAME cppcapture_test COMMAND cppcapture_test)
endif()
