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

		explicit WindowProperties(std::string title = "Goss",
		                     const uint32_t width = 1280,
		                     const uint32_t height = 720)
			: title(std::move(title)), width(width), height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window();

		virtual void Tick() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual bool ShouldClose() const { return false;}

		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetHandle() const = 0;

		static Scope<Window> Create(const WindowProperties& props = WindowProperties());
	};
}
