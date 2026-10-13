# Project configuration
cmake_minimum_required(VERSION 3.18)
project(PatternMarkerGenerator VERSION 1.0 LANGUAGES CXX)

# Set C++ standard for the project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Place executables in build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Load CTest framework and FetchContent module
include(CTest)
include(FetchContent)

# Clipper2 library from GitHub (remote dependency)
FetchContent_Declare(
    Clipper2
    GIT_REPOSITORY https://github.com/AngusJohnson/Clipper2.git
    GIT_TAG        Clipper2_2.0.1
    SOURCE_SUBDIR  CPP 
)

# Do not build Clipper2's examples and tests
set(CLIPPER2_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLIPPER2_TESTS OFF CACHE BOOL "" FORCE)

# Make Clipper2 available to the build system 
FetchContent_MakeAvailable(Clipper2)

# Add subdirectories
add_subdirectory(src)

# Add tests subdirectory only in debug mode
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

#add_subdirectory(python_bindings)