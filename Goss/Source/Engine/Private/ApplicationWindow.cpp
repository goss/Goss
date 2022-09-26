#include "ApplicationWindow.h"

#include <glfw3.h>
#include <iostream>

namespace Goss
{
	ApplicationWindow::ApplicationWindow(const int width, const int height, const char* name): width(width), height(height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, name, nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, FramebufferResizedCallback);

		std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
	}

	ApplicationWindow::~ApplicationWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void ApplicationWindow::CreateWindowSurface(const VkInstance instance, VkSurfaceKHR* surface) const
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface))
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}

	void ApplicationWindow::FramebufferResizedCallback(GLFWwindow* window, const int width, const int height)
	{
		ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
		appWindow->framebufferResized = true;
		appWindow->width = width;
		appWindow->height = height;
	}
}
