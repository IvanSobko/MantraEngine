project(MantraEngine)

set(HEADER_FILES
    "src/MantraEngine.h"
    "src/Mantra/Application.h"
    "src/Mantra/Core.h"
    "src/Mantra/EntryPoint.h"

    "src/Mantra/Events/Event.h"
    "src/Mantra/Events/KeyEvent.h"
    "src/Mantra/Events/MouseEvent.h"
    "src/Mantra/Events/ApplicationEvent.h"

    "src/Mantra/Log/Log.h"

    "src/Mantra/Window/Window.h"
    "src/Mantra/Window/GLFWWindow.h"

    "src/Mantra/Layers/Layer.h"
    "src/Mantra/Layers/Layerstack.h"
    "src/Mantra/Layers/ImGuiLayer.h"

    "src/Mantra/Input/Input.h"
    "src/Mantra/Input/GLFWInput.h"
)

set(PCH_FILES
    "src/ME_PCH.h")

set(SRC_FILES
    "src/Mantra/Application.cpp"

    "src/Mantra/Log/Log.cpp"

    "src/Mantra/Window/GLFWWindow.cpp"

    "src/Mantra/Layers/Layerstack.cpp"
    "src/Mantra/Layers/ImGuiLayer.cpp"

    "src/Mantra/Input/GLFWInput.cpp"
)

set(ALL_FILES
    ${HEADER_FILES}
    ${SRC_FILES}
)

# ===== OPENGL & GLFW & GLAD =====
find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" ON)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/MantraEngine/libs/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/MantraEngine/libs/glad)

# ========== spdlog ==============
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_subdirectory(${CMAKE_SOURCE_DIR}/MantraEngine/libs/spdlog)

# ========== imgui ==============
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/MantraEngine/libs/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)
file(GLOB IMGUI_FILES ${IMGUI_DIR}/*.cpp ${IMGUI_DIR}/*.h)
set(IMGUI_FILES ${IMGUI_FILES}  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp)


set(ALL_FILES ${ALL_FILES} ${IMGUI_FILES})
# ================================

if(MSVC)
    message("Adding MT-DLL options.")
    add_compile_options(
        $<$<CONFIG:>:/MD>
        $<$<CONFIG:Debug>:/MDd> 
        $<$<CONFIG:Release>:/MD>
    )
endif()


add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL glad spdlog::spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/libs/spdlog/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ME_PLATFORM_WINDOWS ME_BUILD_DLL GLFW_INCLUDE_NONE)
