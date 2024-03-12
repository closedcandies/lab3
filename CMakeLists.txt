cmake_minimum_required(VERSION 3.12)

project(
        labwork3-closedcandies
        VERSION 1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

add_executable(labwork3-closedcandies main.cpp
        lib/sandbox.h
        lib/drawer.cpp
        lib/field.cpp
        lib/argparser.cpp
        lib/other-functions.cpp
)
add_subdirectory(lib)
