#pragma once

#include "Event.h"
#include <string>
#include <functional>

namespace Goss
{
	struct WindowProperties
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		explicit WindowProperties(std::string title = "Hazel Engine",
		                     const uint32_t width = 800,
		                     const uint32_t height = 600)
			: title(std::move(title)), width(width), height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window();

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProperties& props = WindowProperties());
	};

}
