add_library(utils_module STATIC
    timer.cpp
    logger_setup.cpp
)

target_include_directories(utils_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libs/spdlog/include
)