add_executable(ravencpp_test 
	main.cpp
)
target_link_libraries(ravencpp_test ravencpp)
add_test(NAME ravencpp_test COMMAND ravencpp_test)
