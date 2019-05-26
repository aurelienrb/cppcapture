if (BUILD_TESTING)
    add_executable(cppcapture_example main.cpp)
    target_link_libraries(cppcapture_example cppcapture)
endif()
