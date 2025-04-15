cmake_minimum_required(VERSION 3.15)
project(UrbanMapProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories if needed:
include_directories(${PROJECT_SOURCE_DIR})

# List your source files (for example, main.cpp)
add_executable(UrbanMapProject
        parsing.cpp
        data_structures/MutablePriorityQueue.h
        #menu.cpp
        # List any additional .cpp files here
)

# If you have subdirectories, add them:
# add_subdirectory(src)
