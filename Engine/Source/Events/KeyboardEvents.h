#pragma once
#include "Events/Event.h"
#include "KeyCodes.h"

class KeyEvent : public Event
{
public:
	KeyCode GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(const KeyCode keyCode): m_KeyCode(keyCode) { }

	KeyCode m_KeyCode{ 32 };
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(const KeyCode keyCode, bool isRepeat = false)
		: KeyEvent(keyCode), m_IsRepeat(isRepeat)
	{ }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
		return ss.str();
	}

	bool IsRepeat() const { return m_IsRepeat; }

	EVENT_CLASS_TYPE(KeyPressed)

private:
	bool m_IsRepeat{ false };
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(const KeyCode keyCode)
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