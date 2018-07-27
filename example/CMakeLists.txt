cmake_minimum_required(VERSION 3.10)
project(test)

set(CMAKE_CXX_STANDARD 11)
set(PROJECT_WTD "${CMAKE_SOURCE_DIR}/..")
set(EXAMPLE_PATH "${PROJECT_WTD}/example")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

include_directories(${EXAMPLE_PATH}/include)
include_directories(${PROJECT_WTD}/include)
include_directories(${PROJECT_WTD}/include/rapidjson)

file(GLOB test-src ${CMAKE_SOURCE_DIR}/*.cpp)
set(test-src ${test-src} "${PROJECT_WTD}/include/slothjson.cpp")
foreach(test-src-item ${test-src})
    message(STATUS ${test-src-item})
endforeach()

add_executable(test ${test-src})
