#pragma once
#include "gepch.h"

#include "Event.h"
#include "KeyCodes.h"

namespace Goss {

	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		explicit KeyEvent(const KeyCode keycode)
			: keyCode(keycode) {}

		KeyCode keyCode;
	};

	class KeyPressedEvent final : public KeyEvent
	{
	public:
		explicit KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
			: KeyEvent(keycode), isRepeated(isRepeat) {}

		bool IsRepeat() const { return isRepeated; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << keyCode << " (repeat = " << isRepeated << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool isRepeated;
	};

	class KeyReleasedEvent final : public KeyEvent
	{
	public:
		explicit KeyReleasedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent final : public KeyEvent
	{
	public:
		explicit KeyTypedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
