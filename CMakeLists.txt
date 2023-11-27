# CMake minimum version
cmake_minimum_required(VERSION 3.5)

set(CMAKE_DEBUG_POSTFIX d)

# Project name / config
project(Complexia)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Test source files
file(GLOB TEST_SOURCES tests/*.c)

# Source files
file(GLOB SOURCES src/*.c)

# C math library (-lm on command-line)
link_libraries(m)

# Include directory
include_directories(include/)
# Iterate over each test source file
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Extract the file name without extension
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # Create an executable for each test source file
    add_executable(tests/${TEST_NAME} ${TEST_SOURCE} ${SOURCES})

    # Optionally, link any necessary libraries
    # target_link_libraries(${TEST_NAME} some_library)
endforeach()