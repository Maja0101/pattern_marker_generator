add_library(nesting 
    placed_instance.cpp
    bottom_left.cpp
)

target_include_directories(nesting PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(nesting PUBLIC 
    pattern 
    geometry
)