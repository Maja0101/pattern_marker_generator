# Project configuration
cmake_minimum_required(VERSION 3.18)
project(PatternMarkerGenerator VERSION 1.0 LANGUAGES CXX)

# Set C++ standard for the project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Settings for static code analysis
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY 
        ${CLANG_TIDY_EXE}
        "-extra-arg=-std=c++20"
    )
    message(STATUS "Clang-Tidy active and integrated with CMake")
else()
    message(WARNING "Clang-Tidy not found - code analysis is disabled")
endif()

# Static libs linked into the Node addon
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Place executables in build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Load CTest framework
include(CTest)

# Node addon
if(CMAKE_JS_VERSION)
    execute_process(
        COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REPLACE "\"" "" NODE_ADDON_API_DIR "${NODE_ADDON_API_DIR}")
    set(NODE_ADDON_API_DIR "${NODE_ADDON_API_DIR}" CACHE INTERNAL "")
endif()

# Clipper2 configuration
set(CLIPPER2_UTILS OFF CACHE BOOL "" FORCE)
set(CLIPPER2_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLIPPER2_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/clipper2/CPP)

# Add subdirectories
add_subdirectory(src)

# Add tests subdirectory only in debug mode
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()