cmake_minimum_required(VERSION 3.5)

add_executable(test_mutex
	test_mutex.cpp
	)
add_executable(test_thread
	test_thread.cpp
	)
target_link_libraries(test_thread core)
add_executable(test_aloop
	test_aloop.cpp
	)
target_link_libraries(test_aloop   core )



add_executable(test_proto
	test_proto.cpp
	)
target_link_libraries(test_proto core )

add_executable(test_atproto
	test_atproto.cpp
	)
target_link_libraries(test_atproto core )

add_executable(test_ble
	test_ble.c
	)
target_link_libraries(test_ble core  -lstdc++)
install(TARGETS test_ble DESTINATION  bin)