#include "gepch.h"

#include "Window.h"
#include "WindowsWindow.h"

namespace Goss
{
	Scope<Window> Window::Create(const WindowProperties& props)
	{
		#ifdef GE_PLATFORM_WINDOWS
			return CreateScope<WindowsWindow>(props);
		#else
			GE_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
		#endif
	}

	Window::~Window() = default;
}
