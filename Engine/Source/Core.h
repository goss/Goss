#pragma once


#ifdef GE_PLATFORM_WINDOWS
	#ifdef GE_BUILD_DLL
	#define GOSS_API __declspec(dllexport)
	#else
	#define GOSS_API __declspec(dllimport)
	#endif
#else
	#error GOSS only supports Windows!
#endif