#include "ApplicationWindow.h"

#include <glfw3.h>
#include <iostream>

namespace Goss
{
	ApplicationWindow::ApplicationWindow(const int width, const int height, const char* name): width(width), height(height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(width, height, name, nullptr, nullptr);

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
}
