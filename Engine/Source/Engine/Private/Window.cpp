#include "Window.h"

#include <glfw3.h>
#include <iostream>

namespace Goss
{
	Window::Window(const int width, const int height, const char* name): width(width), height(height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, name, nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, FramebufferResizedCallback);

		std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
	}

	Window::~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Window::CreateWindowSurface(const VkInstance instance, VkSurfaceKHR* surface) const
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface))
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}

	void Window::FramebufferResizedCallback(GLFWwindow* window, const int width, const int height)
	{
		Window* appWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		appWindow->framebufferResized = true;
		appWindow->width = width;
		appWindow->height = height;
	}
}
