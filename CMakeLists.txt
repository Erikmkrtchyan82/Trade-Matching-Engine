# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(Trade_Matching_Engine VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the source and include directories
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/headers)

# Find all .cpp and .h files
file(GLOB_RECURSE CPP_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${HEADER_DIR}/*.hpp)

# Add the executable target
add_executable(Engine ${CPP_FILES} ${HEADER_FILES})

# Include directories for
target_include_directories(Engine PRIVATE ${HEADER_FILES})
