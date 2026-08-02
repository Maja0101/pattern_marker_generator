# Project configuration
cmake_minimum_required(VERSION 3.18)
project(CppFirst VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Place executables in build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Enable CTest and create configuration files for tests
include(CTest)

# Add subdirectories
add_subdirectory(src)

# Add tests subdirectory only in debug mode
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

#add_subdirectory(python_bindings)