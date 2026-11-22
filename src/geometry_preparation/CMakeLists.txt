add_library(preparation 
    decomposition.cpp
    transformations.cpp
    initialization.cpp
)

target_include_directories(preparation PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${CMAKE_SOURCE_DIR}/libs/mapbox/include
)

target_link_libraries(preparation PUBLIC 
    common
    utils
    Clipper2
)