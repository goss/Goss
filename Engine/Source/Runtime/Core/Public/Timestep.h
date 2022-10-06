#pragma once

namespace Goss
{
	class Timestep
	{
	public:
		Timestep(const float time = 0.0f) : time(time)
		{
		}

		explicit operator float() const { return time; }

		float GetSeconds() const { return time; }
		float GetMilliseconds() const { return time * 1000.0f; }
	private:
		float time;
	};

}