#include "gepch.h"
#include "OrthographicCamera.h"

#include <gtc/matrix_transform.hpp>

namespace Goss
{
	OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top)
	{
		SetProjection(left, right, bottom, top);
	}

	void OrthographicCamera::SetProjection(const float left, const float right, const float bottom, const float top)
	{
		projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		const glm::mat4 transform = translate(glm::mat4(1.0f), position) * rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

		viewMatrix = inverse(transform);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

}