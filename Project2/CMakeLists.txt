cmake_minimum_required(VERSION 3.27)
project(Project2)

set(CMAKE_CXX_STANDARD 20)

#set(ORTOOLS_ROOT "${CMAKE_SOURCE_DIR}/or-tools")

include_directories(
        Parsing
        src
        #${ORTOOLS_ROOT}/include
)

#link_directories(${ORTOOLS_ROOT}/lib)

add_executable(Project2
        Parsing/PalletPacking.h
        Parsing/parsing.cpp
        src/main.cpp
        src/algorithms.h
        src/bruteForce.cpp
        src/greedy.cpp
        src/dynamicP.cpp
        src/ilp.cpp
)

#file(GLOB ORTOOLS_ALL_LIBS "${ORTOOLS_ROOT}/lib/*.lib")
#target_link_libraries(Project2 ${ORTOOLS_ALL_LIBS})