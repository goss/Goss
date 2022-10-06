#pragma once

#include "Window.h"
//#include "GraphicsContext.h"

#include <GLFW/glfw3.h>

#include "GraphicsContext.h"

namespace Goss
{
	class WindowsWindow final : public Window
	{
	public:
		explicit WindowsWindow(const WindowProperties& props);
		~WindowsWindow() override;

		void Tick() override;

		unsigned int GetWidth() const override { return windowData.width; }
		unsigned int GetHeight() const override { return windowData.height; }

		bool ShouldClose() const override { return glfwWindowShouldClose(window);}

		void SetEventCallback(const EventCallbackFunc& callback) override { windowData.eventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		void* GetWindow() const override { return window; }

	private:
		void Init(const WindowProperties& props);
		void Shutdown() const;

		GLFWwindow* window;
		Scope<GraphicsContext> context;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFunc eventCallback;
		};

		WindowData windowData;
	};

}
