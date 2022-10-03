#pragma once

#ifdef GE_PLATFORM_WINDOWS
	#ifndef NOMINMAX
/**
 * On Windows, a header file defines two macros min and max which may result in conflicts with their counterparts in the standard library and therefore in errors during compilation.
 */
#define NOMINMAX
	#endif
#endif

#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#ifdef GE_PLATFORM_WINDOWS 
#include <Windows.h>
		#undef near
		#undef far
#endif


