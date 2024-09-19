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

    # "src/Mantra/Log/Log.h"
)

set(PCH_FILES
    "src/ME_PCH.h")

set(SRC_FILES
    "src/Mantra/Application.cpp"
    # "src/Mantra/Log/Log.cpp"
)

set(ALL_FILES
    ${HEADER_FILES}
    ${SRC_FILES}
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/libs/spdlog/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_FILES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ME_PLATFORM_WINDOWS ME_BUILD_DLL)
