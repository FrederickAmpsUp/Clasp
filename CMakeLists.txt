# CMake minimum version
cmake_minimum_required(VERSION 3.5)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
set(CMAKE_DEBUG_POSTFIX d)

# Project name / config
project(Clasp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
enable_testing()

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

    # Check if the filename contains "exclude" and skip the test if true
    if(NOT ${TEST_SOURCE} MATCHES "dll_test_lib.c")
        # Create an executable for each test source file
        add_executable(${TEST_NAME} ${TEST_SOURCE} ${SOURCES})
        add_test(
            NAME    ${TEST_NAME}
            COMMAND ${TEST_NAME}
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

        # Optionally, link any necessary libraries
        # target_link_libraries(${TEST_NAME} some_library)
    endif()
endforeach()