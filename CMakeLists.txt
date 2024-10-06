cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD 17)

project(Mantra)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4996")

option(MANTRA_BUILD_SANDBOX "Build sandbox application" ON)

add_subdirectory(MantraEngine)
if(MANTRA_BUILD_SANDBOX)
    add_subdirectory(SandboxApp)
endif()
