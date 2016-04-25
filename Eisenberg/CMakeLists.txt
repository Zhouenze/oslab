cmake_minimum_required(VERSION 3.3)
project(Eisenberg)

set(CMAKE_CXX_FLAGS "-Wall -pthread")

set(SOURCE_FILES main.cpp)
add_executable(Eisenberg ${SOURCE_FILES})