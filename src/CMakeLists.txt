set(CPPCAPTURE_INCDIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CPPCAPTURE_SRC
	${CPPCAPTURE_INCDIR}/cppcapture/channel.h
	${CPPCAPTURE_INCDIR}/cppcapture/client.h
	${CPPCAPTURE_INCDIR}/cppcapture/configure.h
	${CPPCAPTURE_INCDIR}/cppcapture/default.h
	${CPPCAPTURE_INCDIR}/cppcapture/eventlevel.h
	${CPPCAPTURE_INCDIR}/cppcapture/event.h
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
	stacktrace.h
	systemutils.h
	systemutils.cpp
)

if (WIN32)
	list(APPEND CPPCAPTURE_SRC
		network/socket_windows.cpp
		network/socket_windows.h
		stacktrace_windows.cpp
		systemutils_windows.cpp
	)
else()
	list(APPEND CPPCAPTURE_SRC
		systemutils_posix.cpp
	)
endif()

add_library(cppcapture ${CPPCAPTURE_SRC})
target_include_directories(cppcapture
PUBLIC
	${CPPCAPTURE_INCDIR}
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)

if (CPPCAPTURE_LOG_TRACES)
	target_compile_definitions(cppcapture PRIVATE -DCPPCAPTURE_LOG_TRACES)
endif()

if (WIN32)
	target_link_libraries(cppcapture StackWalker)
else()
	target_link_libraries(cppcapture pthread)
endif()


if (CPPCAPTURE_BUILD_TESTS)
	add_executable(cppcapture_test 
		stacktrace_test.cpp
		systemutils_test.cpp
		encoder/json_encoder_test.cpp
	)
	target_link_libraries(cppcapture_test cppcapture catch)
endif()
