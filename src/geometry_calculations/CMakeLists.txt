add_library(calculations 
    calculations.cpp
    nfp_ifp_calculations.cpp
    ifp_pocket_calculations.cpp
    options_calculations.cpp
)

target_include_directories(calculations PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${CMAKE_SOURCE_DIR}/libs/mapbox/include
)

target_link_libraries(calculations PUBLIC 
    utils 
    pattern
    common
)