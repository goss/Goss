#pragma once

#include <chrono>

namespace Goss
{
	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Reset()
		{
			startTime = std::chrono::high_resolution_clock::now();
		}

		float Elapsed() const
		{
			return static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count()) * 0.001f * 0.001f * 0.001f;
		}

		float ElapsedMillis() const
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	};

}
