#pragma once

#ifdef ME_PLATFORM_WINDOWS
    #ifdef ME_BUILD_DLL
        #define MANTRA_API __declspec(dllexport)
    #else
        #define MANTRA_API __declspec(dllimport)
    #endif
#else
    #ifdef ME_BUILD_DLL
        #define MANTRA_API __attribute__((visibility("default")))
    #else
        #define MANTRA_API  // no equivalent needed: default visibility is enough
    #endif
#endif

#define BIT(x) (1 << x)

#ifdef WIN32
    #define ME_DEBUGBREAK __debugbreak()
#else
    #include <csignal>
    #define ME_DEBUGBREAK raise(SIGTRAP)
#endif

// should log be included here?
#include "Log/Log.h"

#define ME_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define ME_ASSERT(x, ...) { if(!(x)) { ME_ERROR("Assertion Failed: {0}", __VA_ARGS__); ME_DEBUGBREAK; } }
#define ME_CORE_ASSERT(x, ...) { if(!(x)) { ME_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); ME_DEBUGBREAK; } }