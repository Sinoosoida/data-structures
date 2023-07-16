cmake_minimum_required(VERSION 3.24)
project(memory_manager)

set(CMAKE_CXX_STANDARD 17)

add_executable(memory_manager main.cpp mm.h test.cpp)
