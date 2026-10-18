add_library(geometry 
    svg_exporter.cpp 
    transformations.cpp 
    calculations.cpp
    nfp_ifp_calculations.cpp
    decomposition_calculations.cpp
)

target_include_directories(geometry PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${CMAKE_SOURCE_DIR}/libs/mapbox/include
)

target_link_libraries(geometry PUBLIC 
    nesting 
    pattern
)