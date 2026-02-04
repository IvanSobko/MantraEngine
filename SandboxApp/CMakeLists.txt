project(Sandbox)

set(SRC_FILES
    "src/SandboxApp.cpp"
)

set(ALL_FILES
    ${SRC_FILES}
)

add_executable(${PROJECT_NAME} ${ALL_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../MantraEngine/"
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ME_PLATFORM_WINDOWS)
endif()

# if(UNIX)
#     target_compile_definitions(${PROJECT_NAME} PRIVATE)
# endif()

target_link_libraries(${PROJECT_NAME} PRIVATE MantraEngine)