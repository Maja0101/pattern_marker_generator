add_library(common
    helpers.cpp 
)

target_include_directories(common PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(common PUBLIC 
    Clipper2
)