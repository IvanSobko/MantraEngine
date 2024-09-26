cmake_minimum_required(VERSION 3.16.0)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")

add_subdirectory(MantraEngine)
add_subdirectory(SandboxApp)
