# AI generated CMake files go brrr

cmake_minimum_required(VERSION 3.10)
project(Clasp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directory
include_directories(inc)

# Gather all source files from src/ recursively
file(GLOB_RECURSE SRC_FILES src/*.c src/*.cpp)

add_library(claspSourceLibs STATIC ${SRC_FILES})

file(GLOB LIB_FILES lib/*.a)

# Link each library with the deps target
foreach(LIB_FILE ${LIB_FILES})
    # Extract library name without path and extension
    get_filename_component(LIB_NAME ${LIB_FILE} NAME_WE)
    # Link the library with the deps target
    target_link_libraries(claspSourceLibs :${LIB_NAME}.a)
endforeach()

link_directories(lib/)

# Gather all source files from test/ (non-recursive)
file(GLOB TEST_FILES test/*.c test/*.cpp)

# Create executables for each source file in test/
foreach(TEST_FILE ${TEST_FILES})
    # Get the filename without extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    # Create the executable
    add_executable(${TEST_NAME} ${TEST_FILE})
    # Link the executable with the claspSourceLibs library
    target_link_libraries(${TEST_NAME} claspSourceLibs)
endforeach()

# Gather all source files from run/ (non-recursive)
file(GLOB RUN_FILES run/*.c run/*.cpp)

# Create executables for each source file in run/
foreach(RUN_FILE ${RUN_FILES})
    # Get the filename without extension
    get_filename_component(RUN_NAME ${RUN_FILE} NAME_WE)
    # Create the executable
    add_executable(${RUN_NAME} ${RUN_FILE})
    # Link the executable with the deps library
    target_link_libraries(${RUN_NAME} claspSourceLibs)
endforeach()

# Install the run executables
install(TARGETS ${RUN_NAME} DESTINATION /usr/bin)
