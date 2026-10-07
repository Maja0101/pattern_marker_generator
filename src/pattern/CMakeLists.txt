add_library(pattern_module STATIC 
    piece_base.cpp    
)

target_include_directories(pattern_module PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(pattern_module PUBLIC geometry_module)