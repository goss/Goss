#pragma once

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include <vulkan/vulkan_core.h>

namespace Goss
{
	class ApplicationWindow
	{
	    GLFWwindow* window = nullptr;

		int width;
		int height;
		bool framebufferResized = false;

	public:
	    ApplicationWindow(int width, int height, const char* name);
	    ~ApplicationWindow();

	    ApplicationWindow(const ApplicationWindow &) = delete;
	    ApplicationWindow &operator=(const ApplicationWindow &) = delete;

		bool ShouldClose() const { return glfwWindowShouldClose(window);}
		VkExtent2D GetExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const;

		bool WasWindowResized() const { return framebufferResized;}
		void ResetResizedFlag() { framebufferResized = false;}

	private:
		static void FramebufferResizedCallback(GLFWwindow *window, int width, int height);
	};
}


