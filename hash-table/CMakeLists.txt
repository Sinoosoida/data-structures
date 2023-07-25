cmake_minimum_required(VERSION 3.5)

project(hash-table)
add_compile_options(-DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)

add_executable(hash-table test.cpp)