project(MantraEngine)

set(Header_Files
    "MantraEngine.h"
    "src/Application.h"
    "src/Core.h"
    "src/EntryPoint.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "src/Application.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_compile_definitions(${PROJECT_NAME} PUBLIC ME_PLATFORM_WINDOWS ME_BUILD_DLL)
