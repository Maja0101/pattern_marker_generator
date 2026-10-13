add_library(nesting 
    nested_item.cpp
    bottom_left.cpp
    algorithm_base.cpp
    basic_algorithm.cpp
    calculations.cpp
)

target_include_directories(nesting PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(nesting PUBLIC 
    pattern 
    geometry
    utils
)