project(Sandbox)

set(Source_Files
    "src/SandboxApp.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Source_Files}
)

add_executable(${PROJECT_NAME} ${ALL_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../MantraEngine/"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ME_PLATFORM_WINDOWS)

target_link_libraries(${PROJECT_NAME} PRIVATE MantraEngine)