cmake_minimum_required(VERSION 3.14)

project(NDArray)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

add_executable(NDArray NDArray.cpp)
