project(visclient_test)
SET ( CMAKE_CXX_COMPILER "g++" )
SET ( CMAKE_CXX_STANDARD 11)

if (APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()
if (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
endif()

cmake_minimum_required(VERSION 3.16)

find_package(Boost COMPONENTS system filesystem thread serialization REQUIRED)

SET (visserver_srcs VisServer.cpp ../../src/libsalt/VisMsg.h)
add_executable (VisServer.out ${visserver_srcs})

target_link_libraries (VisServer.out ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SERIALIZATION_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

