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
    "src/Mantra/Window/WindowsWindow.h"
)

set(PCH_FILES
    "src/ME_PCH.h")

set(SRC_FILES
    "src/Mantra/Application.cpp"

    "src/Mantra/Log/Log.cpp"

    "src/Mantra/Window/WindowsWindow.cpp"
)

set(ALL_FILES
    ${HEADER_FILES}
    ${SRC_FILES}
)

# ===== OPENGL & GLFW & GLAD =====
find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

add_subdirectory(libs/glfw)

add_subdirectory(libs/glad)

# =========================

if(MSVC)
    message("Adding MT-DLL options.")
    add_compile_options(
        $<$<CONFIG:>:/MD>
        $<$<CONFIG:Debug>:/MDd> 
        $<$<CONFIG:Release>:/MD>
    )
endif()


add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL glad)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/libs/spdlog/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ME_PLATFORM_WINDOWS ME_BUILD_DLL)
