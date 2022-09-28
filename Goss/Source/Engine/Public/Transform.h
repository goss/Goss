#pragma once

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace Goss
{
	struct Transform
	{
		glm::vec2 translation{}; // (position offset)
		glm::vec2 scale{1.f, 1.f};
		float rotation;

		glm::mat2 Mat2()
		{
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			const glm::mat2 rotMatrix{{c, s}, {-s, c}};
			const glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};

			return rotMatrix * scaleMat;
		}
	};
}
