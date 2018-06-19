set(REVEN_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(RAVENLIB_SRC
	${REVEN_INC_DIR}/raven/client.h
	${REVEN_INC_DIR}/raven/config.h
	${REVEN_INC_DIR}/raven/event.h
	${REVEN_INC_DIR}/raven/raven.h
	client.cpp
	event.cpp
	http_encoder.h
	json_encoder.h
	network_socket.cpp
	network_socket.h
	utils.cpp
	utils.h
)

if (WIN32)
	list(APPEND RAVENLIB_SRC network_socket_windows.h)
else()
	list(APPEND RAVENLIB_SRC network_socket_posix.h)
endif()

add_library(ravencpp ${RAVENLIB_SRC})
target_include_directories(ravencpp PUBLIC ${REVEN_INC_DIR})

if (LOG_DEBUG_MESSAGES)
	target_compile_definitions(ravencpp PRIVATE -DLOG_DEBUG_MESSAGES)
endif()
