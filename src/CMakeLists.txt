# All .cpp i .h files in this directory
file(GLOB SRC_FILES "*.cpp")
file(GLOB HDR_FILES "*.h" "*.hpp")

# Subdirectories
add_subdirectory(utils)

# Create executable from all src files
add_executable(pattern_marker_generator ${SRC_FILES})

# Link static library targets to the executable
target_link_libraries(pattern_marker_generator PRIVATE utils_module)

# Specify include directories to use when compiling 
target_include_directories(pattern_marker_generator PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)
