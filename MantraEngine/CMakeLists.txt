project(MantraEngine)

set(HEADER_FILES
    "MantraEngine.h"
    "src/Application.h"
    "src/Core.h"
    "src/EntryPoint.h"

    "src/Events/Event.h"
    "src/Events/KeyEvent.h"
    "src/Events/MouseEvent.h"
    "src/Events/ApplicationEvent.h"
)

set(PCH_FILES
    "ME_PCH.h")

set(SRC_FILES
    "src/Application.cpp"
)

set(ALL_FILES
    ${HEADER_FILES}
    ${SRC_FILES}
)


add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_FILES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ME_PLATFORM_WINDOWS ME_BUILD_DLL)
