cmake_minimum_required(VERSION 3.27)
project(Project2)

set(CMAKE_CXX_STANDARD 14)

include_directories(Parsing)
include_directories(src)

add_executable(Project2
        Parsing/PalletPacking.h
        Parsing/parsing.cpp
        src/main.cpp
        src/algorithms.h
        src/bruteForce.cpp
        src/greedy.cpp
        src/dynamicP.cpp)
