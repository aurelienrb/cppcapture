set(REVEN_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(RAVENLIB_SRC
	${REVEN_INC_DIR}/raven/channel.h
	${REVEN_INC_DIR}/raven/client.h
	${REVEN_INC_DIR}/raven/config.h
	${REVEN_INC_DIR}/raven/eventlevel.h
	${REVEN_INC_DIR}/raven/event.h
	${REVEN_INC_DIR}/raven/raven.h
	channels/sentry.cpp
	channels/sentry.h
	channels/socket_posix.h
	channels/socket_windows.h
	channels/socket.cpp
	channels/socket.h
	channel.cpp
	client.cpp
	event.cpp
	json_encoder.h
	raven.cpp
	utils.h
)

add_library(ravencpp ${RAVENLIB_SRC})
target_include_directories(ravencpp PUBLIC ${REVEN_INC_DIR})

if (LOG_DEBUG_MESSAGES)
	target_compile_definitions(ravencpp PRIVATE -DLOG_DEBUG_MESSAGES)
endif()
