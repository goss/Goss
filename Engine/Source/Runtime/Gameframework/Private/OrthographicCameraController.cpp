#include "gepch.h"
#include "OrthographicCameraController.h"

#include "ApplicationEvent.h"
#include "Event.h"
#include "Input.h"
#include "MouseEvent.h"
#include "Timestep.h"

namespace Goss
{
	OrthographicCameraController::OrthographicCameraController(const float aspectRatio, const bool rotation)
		: aspectRatio(aspectRatio), camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel), rotation(rotation)
	{
	}

	void OrthographicCameraController::Tick(const Timestep timeStep)
	{
		if (Input::IsKeyPressed(Key::W))
		{
			cameraPosition.x += -sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timeStep;
			cameraPosition.y += cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timeStep;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			cameraPosition.x -= -sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timeStep;
			cameraPosition.y -= cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timeStep;
		}

		if (Input::IsKeyPressed(Key::A))
		{
			cameraPosition.x -= cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timeStep;
			cameraPosition.y -= sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timeStep;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			cameraPosition.x += cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timeStep;
			cameraPosition.y += sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timeStep;
		}

		if (rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				cameraRotation += cameraRotationSpeed * timeStep;
			if (Input::IsKeyPressed(Key::E))
				cameraRotation -= cameraRotationSpeed * timeStep;

			if (cameraRotation > 180.0f)
				cameraRotation -= 360.0f;
			else if (cameraRotation <= -180.0f)
				cameraRotation += 360.0f;

			camera.SetRotation(cameraRotation);
		}

		camera.SetPosition(cameraPosition);

		cameraTranslationSpeed = zoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(const float width, const float height)
	{
		aspectRatio = width / height;
		camera.SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(const MouseScrolledEvent& e)
	{
		zoomLevel -= e.GetYOffset() * 0.25f;
		zoomLevel = std::max(zoomLevel, 0.25f);
		camera.SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(const WindowResizeEvent& e)
	{
		OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
		return false;
	}
}
