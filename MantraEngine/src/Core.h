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