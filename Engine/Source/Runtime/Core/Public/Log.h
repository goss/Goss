#pragma once
#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/string_cast.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace Goss
{
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return coreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return clientLogger; }
	private:
		static Ref<spdlog::logger> coreLogger;
		static Ref<spdlog::logger> clientLogger;
	};

}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define GE_CORE_TRACE(...)    ::Goss::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_INFO(...)     ::Goss::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_WARN(...)     ::Goss::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_ERROR(...)    ::Goss::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_CRITICAL(...) ::Goss::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define GE_TRACE(...)         ::Goss::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GE_INFO(...)          ::Goss::Log::GetClientLogger()->info(__VA_ARGS__)
#define GE_WARN(...)          ::Goss::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GE_ERROR(...)         ::Goss::Log::GetClientLogger()->error(__VA_ARGS__)
#define GE_CRITICAL(...)      ::Goss::Log::GetClientLogger()->critical(__VA_ARGS__)

