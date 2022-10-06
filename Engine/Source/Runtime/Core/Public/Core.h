#pragma once
#include "Platform.h"

#include <memory>
#include <filesystem>

#ifdef GE_PLATFORM_WINDOWS
	#ifdef GE_BUILD_DLL
	#define GOSS_API __declspec(dllexport)
	#else
	#define GOSS_API __declspec(dllimport)
	#endif
#else
	#error GOSS only supports Windows!
#endif

#ifdef GE_DEBUG
	#if defined(GE_PLATFORM_WINDOWS)
		#define GE_DEBUG_BREAK() __debugbreak()
	#elif defined(GE_PLATFORM_LINUX)
		#include <signal.h>
		#define GE_DEBUG_BREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define GE_ENABLE_ASSERTS
#else
	#define GE_DEBUG_BREAK()
#endif

#define GE_EXPAND_MACRO(x) x
#define GE_STRINGIFY_MACRO(x) #x

#ifdef GE_ENABLE_ASSERTS
	#define GE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { GE##type##ERROR(msg, __VA_ARGS__); GE_DEBUG_BREAK(); } }
	#define GE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) GE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define GE_INTERNAL_ASSERT_NO_MSG(type, check) GE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", GE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define GE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define GE_INTERNAL_ASSERT_GET_MACRO(...) GE_EXPAND_MACRO( GE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, GE_INTERNAL_ASSERT_WITH_MSG, GE_INTERNAL_ASSERT_NO_MSG) )

	#define GE_ASSERT(...) GE_EXPAND_MACRO( GE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define GE_CORE_ASSERT(...) GE_EXPAND_MACRO( GE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define GE_ASSERT(...)
	#define GE_CORE_ASSERT(...)
#endif

#define BIT(x) (1 << (x))
#define GE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Goss
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}