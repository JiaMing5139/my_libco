cmake_minimum_required(VERSION 3.15)
project(my_libco)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -O1)

add_executable(my_libco main.cpp util.h util.cc co_routine.cpp co_routine.h noncopyable.cpp noncopyable.h )