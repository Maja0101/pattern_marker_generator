# Add subdirectories
add_subdirectory(utils)
add_subdirectory(nesting)
add_subdirectory(pattern)
add_subdirectory(geometry)

add_library(facade STATIC
	api/nesting_api.cpp 
)

# Link library targets
target_link_libraries(facade PUBLIC 
    Clipper2
	utils
	nesting
	pattern
	geometry
)

target_include_directories(facade PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Create executable from main
add_executable(pattern_marker_generator main.cpp sample_input.cpp)

target_link_libraries(pattern_marker_generator PRIVATE facade)

if(CMAKE_JS_VERSION)
    add_library(pmg_engine MODULE bridge/addon_bridge.cpp)

    target_include_directories(pmg_engine PRIVATE
        ${NODE_ADDON_API_DIR}
        ${CMAKE_JS_INC}
    )

    target_compile_definitions(pmg_engine PRIVATE 
        NAPI_VERSION=8 
        NAPI_CPP_EXCEPTIONS
    )

    target_link_libraries(pmg_engine PRIVATE facade ${CMAKE_JS_LIB})

    set_target_properties(pmg_engine PROPERTIES PREFIX "" SUFFIX ".node")

    if(MSVC)
        target_compile_options(pmg_engine PRIVATE /EHsc)
    else()
        target_compile_options(pmg_engine PRIVATE -fexceptions)
    endif()
endif()
