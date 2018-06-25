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
	network/socket_posix.h
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
	list(APPEND cppcapture_SRC
		network/socket_windows.cpp
		network/socket_windows.h
		systemutils_windows.cpp
	)
else()
	list(APPEND cppcapture_SRC
		systemutils_posix.cpp
	)
endif()

add_library(cppcapture ${cppcapture_SRC})
target_include_directories(cppcapture PUBLIC ${cppcapture_INC_DIR})
target_include_directories(cppcapture PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (CPPCAPTURE_LOG_TRACES)
	target_compile_definitions(cppcapture PRIVATE -DCPPCAPTURE_LOG_TRACES)
endif()

if (UNIX)
	target_link_libraries(cppcapture pthread)
endif()


if (CPPCAPTURE_BUILD_TESTS)
	add_executable(cppcapture_test 
		systemutils_test.cpp
	)
	target_link_libraries(cppcapture_test cppcapture catch)
endif()
