#pragma once
#include "Events/Event.h"
#include "KeyCodes.h"

class KeyEvent : public Event
{
public:
	int GetKeyCode() const { return _keyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(const int keyCode): _keyCode(keyCode) { }

	int _keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(const int keyCode, bool isRepeat = false)
		: KeyEvent(keyCode), _bIsRepeat(isRepeat)
	{ }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << _keyCode << " (repeat = " << _bIsRepeat << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)

private:
	bool _bIsRepeat{ false };
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(const int keyCode)
		: KeyEvent(keyCode)
	{}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << _keyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(const int keyCode)
		: KeyEvent(keyCode) { }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << _keyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};