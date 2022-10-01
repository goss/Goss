#pragma once
#include <memory>

#ifdef GE_PLATFORM_WINDOWS
	#ifdef GE_BUILD_DLL
	#define GOSS_API __declspec(dllexport)
	#else
	#define GOSS_API __declspec(dllimport)
	#endif
#else
	#error GOSS only supports Windows!
#endif

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
