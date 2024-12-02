# Inside test/CMakeLists.txt

if(NOT TARGET GTest::gtest)
  # Fetch GoogleTest if not already available
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

# Find all test files ending with "test.cpp" in the test/ directory
file(GLOB TEST_SOURCES "test*.cpp")

add_executable(
  test_all
  ${TEST_SOURCES}
)

# Link GoogleTest to the test_all executable
target_link_libraries(
  test_all
  GTest::gtest_main
)

# Add include directories if needed
target_include_directories(
  test_all
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

# Discover tests using GoogleTest
include(GoogleTest)
gtest_discover_tests(test_all)

# For each test source, create an executable
foreach(TEST_SOURCE ${TEST_SOURCES})
  # Get the name of the test file without the directory and extension
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

  # Add the test executable
  add_executable(
    ${TEST_NAME}
    ${TEST_SOURCE}
  )

  # Link GoogleTest to the test executable
  target_link_libraries(
    ${TEST_NAME}
    GTest::gtest_main
  )

  # Add include directories if needed
  target_include_directories(
    ${TEST_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
  )

  # Discover tests using GoogleTest
  include(GoogleTest)
  gtest_discover_tests(${TEST_NAME})
endforeach()
