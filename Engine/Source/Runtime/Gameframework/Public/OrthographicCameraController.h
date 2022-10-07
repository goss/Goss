#pragma once
#include "ApplicationEvent.h"
#include "Event.h"
#include "MouseEvent.h"
#include "OrthographicCamera.h"
#include "Timestep.h"

namespace Goss
{
	class OrthographicCameraController
	{
	public:
		explicit OrthographicCameraController(float aspectRatio, bool rotation = false);

		void Tick(Timestep timeStep);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return camera; }
		const OrthographicCamera& GetCamera() const { return camera; }

		float GetZoomLevel() const { return zoomLevel; }
		void SetZoomLevel(const float level) { zoomLevel = level; }
	private:
		bool OnMouseScrolled(const MouseScrolledEvent& e);
		bool OnWindowResized(const WindowResizeEvent& e);
	private:
		float aspectRatio;
		float zoomLevel = 1.0f;
		OrthographicCamera camera;

		bool rotation;

		glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		float cameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float cameraTranslationSpeed = 5.0f;
		float cameraRotationSpeed = 180.0f;
	};

}
