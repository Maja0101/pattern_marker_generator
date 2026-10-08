add_library(geometry_module STATIC 
    svg_exporter.cpp
    transformations.cpp    
)

target_include_directories(geometry_module PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)