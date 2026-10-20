# Add subdirectories
add_subdirectory(utils)
add_subdirectory(nesting)
add_subdirectory(pattern)
add_subdirectory(geometry)

# Create executable from main
add_executable(pattern_marker_generator main.cpp
	sample_input.cpp)

# Link library targets to the executable
target_link_libraries(pattern_marker_generator PRIVATE 
	Clipper2
	utils
	nesting
	pattern
	geometry
)



