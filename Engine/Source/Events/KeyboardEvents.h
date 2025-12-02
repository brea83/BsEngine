#pragma once
#include "Events/Event.h"
#include "KeyCodes.h"

class KeyEvent : public Event
{
public:
	int GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(const int keyCode): m_KeyCode(keyCode) { }

	int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(const int keyCode, bool isRepeat = false)
		: KeyEvent(keyCode), m_IsRepeat(isRepeat)
	{ }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)

private:
	bool m_IsRepeat{ false };
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
		ss << "KeyReleasedEvent: " << m_KeyCode;
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
		ss << "KeyTypedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};