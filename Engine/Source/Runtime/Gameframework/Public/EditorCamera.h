#pragma once

#include "Camera.h"

#include <glm.hpp>

#include "Event.h"
#include "MouseEvent.h"
#include "Timestep.h"

namespace Goss
{
	//TODO make a projection camera
	class EditorCamera final : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep timeStep);
		void OnEvent(Event& e);

		float GetDistance() const { return distance; }
		void SetDistance(const float value) { distance = value; }

		void SetViewportSize(const float width, const float height) { viewportWidth = width; viewportHeight = height; UpdateProjection(); }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return pitch; }
		float GetYaw() const { return yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(const MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		static float RotationSpeed();
		float ZoomSpeed() const;
	private:
		float fov = 45.0f, aspectRatio = 1.778f, nearClip = 0.1f, farClip = 1000.0f;

		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 focalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 initialMousePosition = { 0.0f, 0.0f };

		float distance = 10.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;

		float viewportWidth = 1280;
		float viewportHeight = 720;
	};

}
