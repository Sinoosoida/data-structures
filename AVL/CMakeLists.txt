cmake_minimum_required(VERSION 3.24)
project(AVL)

include_directories(.)

add_library(avl avltree.h)
set_target_properties(avl PROPERTIES LINKER_LANGUAGE CXX)


add_library(mm mm.h)
set_target_properties(mm PROPERTIES LINKER_LANGUAGE CXX)


add_executable(AVL test.cpp)

target_link_libraries(AVL avl mm)

add_test(NAME AVL
        COMMAND AVL)