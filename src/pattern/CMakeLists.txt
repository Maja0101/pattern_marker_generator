add_library(pattern_module STATIC 
    pattern_piece.cpp    
)

target_link_libraries(pattern_module PUBLIC geometry_module)