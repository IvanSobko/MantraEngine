project(MantraEngine)

file(GLOB_RECURSE ENGINE_SOURCES 
    "src/*.cpp"
    "src/*.c"
)

file(GLOB_RECURSE ENGINE_HEADERS 
    "src/*.h"
    "src/*.hpp"
)

# ===== OPENGL & GLFW & GLAD & GLM =====
find_package(OpenGL REQUIRED)

option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" ON)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/MantraEngine/libs/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/MantraEngine/libs/glad)
add_subdirectory(${CMAKE_SOURCE_DIR}/MantraEngine/libs/glm)

# ========== spdlog ==============
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
add_subdirectory(${CMAKE_SOURCE_DIR}/MantraEngine/libs/spdlog)

# ========== imgui ==============
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/MantraEngine/libs/imgui)
file(GLOB IMGUI_FILES ${IMGUI_DIR}/*.cpp ${IMGUI_DIR}/*.h)
set(IMGUI_FILES ${IMGUI_FILES} 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp  
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp)

# ========== stb_image ==============
set(STB_DIR ${CMAKE_SOURCE_DIR}/MantraEngine/libs/stb_image)
file(GLOB STB_FILES ${STB_DIR}/*.cpp ${STB_DIR}/*.h)

# ================================

if(MSVC)
    message("Adding MT-DLL options.")
    add_compile_options(
        $<$<CONFIG:>:/MD>
        $<$<CONFIG:Debug>:/MDd> 
        $<$<CONFIG:Release>:/MD>
    )
endif()

# Create library with all collected files
add_library(${PROJECT_NAME} SHARED 
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
    ${IMGUI_FILES}
    ${STB_FILES}
)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL glad spdlog::spdlog glm::glm)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${IMGUI_DIR} 
    ${IMGUI_DIR}/backends
    ${PROJECT_SOURCE_DIR}/libs/spdlog/include
    ${PROJECT_SOURCE_DIR}/src
    ${STB_DIR}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE "src/ME_PCH.h")

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ME_PLATFORM_WINDOWS ME_BUILD_DLL GLFW_INCLUDE_NONE ME_DEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE ME_BUILD_DLL GLFW_INCLUDE_NONE ME_DEBUG) # GLFW_INCLUDE_NONE to force not include opengl in glfw files
endif()
