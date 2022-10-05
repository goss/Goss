#include "gepch.h"
#include "EditorCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <fwd.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtx/quaternion.hpp>

#include "Event.h"
#include "Input.h"
#include "MouseEvent.h"
#include "Timestep.h"

namespace Goss {

	EditorCamera::EditorCamera(const float fov, const float aspectRatio, const float nearClip, const float farClip)
		: Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)), fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip)
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		aspectRatio = viewportWidth / viewportHeight;
		projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
	}

	void EditorCamera::UpdateView()
	{
		// yaw = pitch = 0.0f; // Lock the camera's rotation
		position = CalculatePosition();

		const glm::quat orientation = GetOrientation();
		viewMatrix = translate(glm::mat4(1.0f), position) * toMat4(orientation);
		viewMatrix = inverse(viewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		const float x = std::min(viewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		const float y = std::min(viewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed()
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float d = distance * 0.2f;
		d = std::max(d, 0.0f);
		float speed = d * d;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnUpdate(Timestep timeStep)
	{
		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			const glm::vec2 delta = (mouse - initialMousePosition) * 0.003f;
			initialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GE_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(const MouseScrolledEvent& e)
	{
		const float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		focalPoint += -GetRightDirection() * delta.x * xSpeed * distance;
		focalPoint += GetUpDirection() * delta.y * ySpeed * distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		yaw += yawSign * delta.x * RotationSpeed();
		pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(const float delta)
	{
		distance -= delta * ZoomSpeed();
		if (distance < 1.0f)
		{
			focalPoint += GetForwardDirection();
			distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return focalPoint - GetForwardDirection() * distance;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat{glm::vec3(-pitch, -yaw, 0.0f)};
	}

}
