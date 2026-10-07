add_library(nesting_module STATIC 
    placed_instance.cpp    
)

target_link_libraries(nesting_module PUBLIC pattern_module)