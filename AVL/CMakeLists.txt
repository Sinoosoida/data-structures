cmake_minimum_required(VERSION 3.24)
project(AVL)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
add_executable(AVL test.cpp)