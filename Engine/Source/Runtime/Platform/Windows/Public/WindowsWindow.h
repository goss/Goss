#pragma once

#include "Window.h"
#include "GraphicsContext.h"

#include <GLFW/glfw3.h>

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

		bool ShouldClose() const override { return glfwWindowShouldClose(windowHandle);}

		void SetEventCallback(const EventCallbackFunc& callback) override { windowData.eventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		void* GetHandle() const override { return windowHandle; }

	private:
		void Initialize(const WindowProperties& props);
		void Shutdown() const;

		GLFWwindow* windowHandle;
		Scope<GraphicsContext> context;

		struct WindowData
		{
			std::string title;
			unsigned int width = 0, height = 0;
			bool vSync;

			EventCallbackFunc eventCallback;
		};

		WindowData windowData;
	};
}
