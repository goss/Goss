#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vulkan/vulkan_core.h>


namespace Goss
{
	class Window
	{
	    GLFWwindow* window = nullptr;

		int width;
		int height;
		bool framebufferResized = false;

	public:
	    Window(int width, int height, const char* name);
	    ~Window();

	    Window(const Window &) = delete;
	    Window &operator=(const Window &) = delete;

		bool ShouldClose() const { return glfwWindowShouldClose(window);}
		VkExtent2D GetExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const;

		bool WasWindowResized() const { return framebufferResized;}
		void ResetResizedFlag() { framebufferResized = false;}

	private:
		static void FramebufferResizedCallback(GLFWwindow *window, int width, int height);
	};
}


