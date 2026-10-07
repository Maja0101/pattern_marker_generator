add_library(geometry_module INTERFACE)

target_include_directories(geometry_module INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)