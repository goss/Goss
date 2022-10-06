#include "gepch.h"
#include "WindowsWindow.h"

#include "ApplicationEvent.h"
#include "Core.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "Renderer.h"

namespace Goss
{
	
	static uint8_t glfwWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		GE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProperties& props)
	{
		windowData.title = props.title;
		windowData.width = props.width;
		windowData.height = props.height;

		GE_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if (glfwWindowCount == 0)
		{
			const int success = glfwInit();
			GE_CORE_ASSERT(success, "Could not initialize GLFW!")
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
		#if defined(GE_DEBUG)
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			{
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			}
		#endif
			window = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), windowData.title.c_str(), nullptr, nullptr);
			++glfwWindowCount;
		}

		context = GraphicsContext::Create(window);
		context->Init();

		glfwSetWindowUserPointer(window, &windowData);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(window, [](GLFWwindow* glfwWindow, const int width, const int height)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* glfwWindow)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));
			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* glfwWindow, const int key, int scancode, const int action, int mods)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, false);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.eventCallback(event);
					break;
				}
			default: ;
			}
		});

		glfwSetCharCallback(window, [](GLFWwindow* glfwWindow, const unsigned int keycode)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));

			KeyTypedEvent event(keycode);
			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* glfwWindow, const int button, const int action, int mods)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.eventCallback(event);
					break;
				}
			default: ;
			}
		});

		glfwSetScrollCallback(window, [](GLFWwindow* glfwWindow, const double xOffset, const double yOffset)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* glfwWindow, const double xPos, const double yPos)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.eventCallback(event);
		});
	}

	void WindowsWindow::Shutdown() const
	{
		glfwDestroyWindow(window);
		--glfwWindowCount;

		if (glfwWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::Tick()
	{
		glfwPollEvents();
		context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(const bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		windowData.vSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return windowData.vSync;
	}
}
