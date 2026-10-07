# Add subdirectories
add_subdirectory(utils)
add_subdirectory(geometry)
add_subdirectory(pattern)
add_subdirectory(nesting)

# Create executable from all src files
add_executable(pattern_marker_generator main.cpp)

# Link static library targets to the executable
target_link_libraries(pattern_marker_generator PRIVATE 
	utils_module
	nesting_module
)
