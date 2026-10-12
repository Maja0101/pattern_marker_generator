add_library(pattern 
    pattern_piece.cpp
    shapes.cpp
)

target_include_directories(pattern PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(pattern PUBLIC 
    geometry 
    utils
    Clipper2
)