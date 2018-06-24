set(REVEN_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(RAVENLIB_SRC
	${REVEN_INC_DIR}/raven/channel.h
	${REVEN_INC_DIR}/raven/client.h
	${REVEN_INC_DIR}/raven/config.h
	${REVEN_INC_DIR}/raven/eventlevel.h
	${REVEN_INC_DIR}/raven/event.h
	${REVEN_INC_DIR}/raven/raven.h
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
	debugging.h
	event.cpp
	raven.cpp
	sender_thread.cpp
	sender_thread.h
	systemutils.h
	systemutils.cpp
)

if (WIN32)
	list(APPEND RAVENLIB_SRC
		network/socket_windows.cpp
		network/socket_windows.h
		systemutils_windows.cpp
	)
else()
	list(APPEND RAVENLIB_SRC
		systemutils_posix.cpp
	)
endif()

add_library(ravencpp ${RAVENLIB_SRC})
target_include_directories(ravencpp PUBLIC ${REVEN_INC_DIR})
target_include_directories(ravencpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (LOG_TRACE_MESSAGES)
	target_compile_definitions(ravencpp PRIVATE -DLOG_TRACE_MESSAGES)
endif()

if (UNIX)
	target_link_libraries(ravencpp pthread)
endif()


if (BUILD_TESTS)
	add_executable(ravencpp_test 
		systemutils_test.cpp
	)
	target_link_libraries(ravencpp_test ravencpp catch)
endif()
