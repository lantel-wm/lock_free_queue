cmake_minimum_required(VERSION 3.14)
project(lock_free_queue)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Option to enable or disable tests
option(BUILD_TESTS "Build tests" ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add subdirectory for tests
if(BUILD_TESTS)
  add_subdirectory(test)
endif()

# Main executable
add_executable(
  main
  src/main.cpp
)
