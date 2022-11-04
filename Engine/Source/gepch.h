#pragma once
/*
 * Goss Engine precompile header
 */

#include "Platform.h"

#ifdef GE_PLATFORM_WINDOWS
/**
 * On Windows, a header file defines two macros min and max which may result in conflicts with their counterparts in the standard library and therefore in errors during compilation.
 */
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

// std and stl
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Goss Engine
#include "Log.h"

#ifdef GE_PLATFORM_WINDOWS 
#include <commdlg.h>
#include <Windows.h>
#undef near
#undef far
#endif


