cmake_minimum_required(VERSION 3.3)
project(PV)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES main.cpp)
add_executable(PV ${SOURCE_FILES})