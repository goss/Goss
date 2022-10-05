#pragma once

#include <glm.hpp>

#include "Camera.h"

namespace Goss
{
	class OrthographicCamera final : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return position; }
		void SetPosition(const glm::vec3& pos) { position = pos; RecalculateViewMatrix(); }

		float GetRotation() const { return rotation; }
		void SetRotation(const float rot) { rotation = rot; RecalculateViewMatrix(); }
	private:
		void RecalculateViewMatrix();

		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		float rotation = 0.0f;
	};

}
