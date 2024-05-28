# No shame in AI generated CMake files, right?

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Clasp)

# Add the directory for include files
include_directories(inc)

# Get all source files in the src directory
file(GLOB SRC_FILES src/*.cpp)

# Add a library target with the source files from the src directory
add_library(clasp_deps STATIC ${SRC_FILES})

# Get all source files in the run directory
file(GLOB RUN_FILES run/*.cpp)

# Loop over each source file in the run directory
foreach(RUN_FILE ${RUN_FILES})
    # Get the filename without the directory
    get_filename_component(EXE_NAME ${RUN_FILE} NAME_WE)

    # Create an executable target for each source file
    add_executable(${EXE_NAME} ${RUN_FILE})

    # Link the library to the executable
    target_link_libraries(${EXE_NAME} clasp_deps)

    # Specify the install destination for each executable
    install(TARGETS ${EXE_NAME} DESTINATION /usr/bin)
endforeach()


