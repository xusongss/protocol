cmake_minimum_required(VERSION 3.5)
include (CheckIncludeFiles)
add_definitions(-Dandroid=Inspiry)
##atomic.c need this
#add_definitions(-D__x86_64__ -g)

##定义内存屏障 http://blog.csdn.net/u013234805/article/details/24796503
add_definitions(-DANDROID_SMP=0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
CHECK_INCLUDE_FILES("pthread.h" HAVE_PTHREADS)
CHECK_INCLUDE_FILES("sys/uio.h" HAVE_SYS_UIO_H)
CHECK_INCLUDE_FILES("endian.h" HAVE_ENDIAN_H)
add_definitions(-DHAVE_POSIX_CLOCKS)

if(HAVE_PTHREADS)
	add_definitions(-DHAVE_PTHREADS)
	message(STATUS "------have pthread.h")
endif()
if(HAVE_SYS_UIO_H)
	add_definitions(-DHAVE_SYS_UIO_H)
	message(STATUS "------have sys/uio.h")
endif()
if(HAVE_ENDIAN_H)
	add_definitions(-DHAVE_ENDIAN_H)
	message(STATUS "------have endian.h")
endif()


add_library(core SHARED
	libcutils/properties.c
	libcutils/atomic.c

	libutils/Threads.cpp
	libutils/RefBase.cpp
	libutils/TextOutput.cpp
	libutils/Debug.cpp
	libutils/Timers.cpp
	libutils/VectorImpl.cpp
	libutils/SharedBuffer.cpp
	
	liblog/logd_write.c
	liblog/logprint.c
	liblog/event_tag_map.c
	
	libfoundation/AAtomizer.cpp                 
	libfoundation/ABitReader.cpp                
	libfoundation/ABuffer.cpp                   
	libfoundation/AHandler.cpp                  
	libfoundation/AHierarchicalStateMachine.cpp 
	libfoundation/ALooper.cpp                   
	libfoundation/ALooperRoster.cpp             
	libfoundation/AMessage.cpp                  
	libfoundation/AString.cpp                   
	libfoundation/base64.cpp                    
	libfoundation/hexdump.cpp
	libfoundation/ACrc16CCIT.cpp
	
	libproto/Proto.cpp
	libproto/Com.cpp
	libproto/BaseProto.cpp
	libproto/MacSniffingProto.cpp
	libproto/BLEControlProto.cpp
	libproto/BLEControlHandler.cpp
	libproto/InspiryBLEControl.cpp
	libproto/ATProto.cpp
	)
target_link_libraries(core   pthread rt)
add_subdirectory(test)
install(TARGETS core DESTINATION  lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/proto/InspiryBLEControl.h  DESTINATION include/proto)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/proto/Inspirydefs.h DESTINATION  include/proto )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/test/test_ble.c DESTINATION  example )




