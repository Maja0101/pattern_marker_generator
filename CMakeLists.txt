# Project configuration
cmake_minimum_required(VERSION 3.18)
project(PatternMarkerGenerator VERSION 1.0 LANGUAGES CXX)

# Set C++ standard for the project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static libs linked into the Node addon
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Place executables in build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Load CTest framework
include(CTest)

# Optimize linked code flags
if (MSVC)
    add_compile_options(/Gy)
    add_link_options(/OPT:REF /OPT:ICF)
else()
    add_compile_options(-ffunction-sections -fdata-sections)
    if(APPLE)
        add_link_options("-Wl,-dead_strip")
    else()
        add_link_options("-Wl,--gc-sections")
    endif()
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

# Do not build Clipper2's examples and tests
set(CLIPPER2_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLIPPER2_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/clipper2/CPP)

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

# Add subdirectories
add_subdirectory(src)

# Add tests subdirectory only in debug mode
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()