# Specify path to directory with Catch2
set(CATCH2_SOURCE_DIR "${CMAKE_SOURCE_DIR}/libs")

# Show error when no Catch2 file present
if(NOT EXISTS "${CATCH2_SOURCE_DIR}/catch2/catch_amalgamated.hpp")
  message(FATAL_ERROR
    "${CATCH2_SOURCE_DIR}/catch2/catch_amalgamated.hpp not found"
    "Make sure Catch2 is added in libs/catch2"
  )
endif()

# All test .cpp files
file(GLOB TEST_FILES CONFIGURE_DEPENDS "test_*.cpp")

if(TEST_FILES)
  # Create test executable
  add_executable(unit_tests
    ${TEST_FILES}
    ${CATCH2_SOURCE_DIR}/catch2/catch_amalgamated.cpp
  )

  # Specify include directories to use when compiling 
  target_include_directories(unit_tests PRIVATE ${CATCH2_SOURCE_DIR})
  
  # Link libraries needed for testing
  target_link_libraries(unit_tests PRIVATE 
  nesting 
  Clipper2
  )
  
  # Add a test to the project
  add_test(NAME unit_tests COMMAND unit_tests)
endif()
