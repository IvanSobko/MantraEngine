#pragma once

#ifdef ME_PLATFORM_WINDOWS
    #ifdef ME_BUILD_DLL
        #define MANTRA_API __declspec(dllexport)
    #else
        #define MANTRA_API __declspec(dllimport)
    #endif
#else
    #error Mantra Engine only supports Windows!
#endif

#define BIT(x) (1 << x)

#define ME_ASSERT(x, ...) { if(!(x)) { ME_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define ME_CORE_ASSERT(x, ...) { if(!(x)) { ME_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }