#pragma once

#include <glm.hpp>

namespace Goss
{
	class Camera
	{
	public:
		Camera() = default;

		explicit Camera(const glm::mat4& projection)
			: projectionMatrix(projection), viewProjectionMatrix()
		{
		}

		virtual ~Camera() = default;

		const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }

	protected:
		glm::mat4 projectionMatrix{1.0f};
		glm::mat4 viewMatrix{1.0f};
		glm::mat4 viewProjectionMatrix{1.0f};
	};
}