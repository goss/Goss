#include "gepch.h"

#include "VulkanWindow.h"

#include "Core.h"

namespace Goss
{
	VulkanWindow::VulkanWindow(const int width, const int height, const char* name): width(width), height(height)
	{
		const int results = glfwInit();
		GE_CORE_ASSERT(1, "Could not intialize GLFW")

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, name, nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, FramebufferResizedCallback);

		GE_CORE_INFO("GLFW Version: {}", glfwGetVersionString());
	}

	VulkanWindow::~VulkanWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void VulkanWindow::CreateWindowSurface(const VkInstance instance, VkSurfaceKHR* surface) const
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface))
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}

	void VulkanWindow::FramebufferResizedCallback(GLFWwindow* window, const int width, const int height)
	{
		VulkanWindow* appWindow = static_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
		appWindow->framebufferResized = true;
		appWindow->width = width;
		appWindow->height = height;
	}
}
