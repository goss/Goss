#include "gepch.h"

#include "Window.h"

namespace Goss
{
	Scope<Window> Window::Create(const WindowProperties& props)
	{
		//#ifdef GE_PLATFORM_WINDOWS
		//	return CreateScope<Window>(props); //TODO Create Vulkan Window
		//#else
		//	GE_CORE_ASSERT(false, "Unknown platform!");
		//	return nullptr;
		//#endif

		return nullptr;
	}

	Window::~Window()
	{
	}

}