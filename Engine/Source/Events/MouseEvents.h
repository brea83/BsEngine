#pragma once
#include "BsPrecompileHeader.h"
#include "Events/Event.h"
#include "MouseCodes.h"

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(const float x, const float y)
	: _xPosition(x), _yPosition(y)
	{ }

	float GetX() const { return _xPosition; }
	float GetY() const { return _yPosition; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << _xPosition << ", " << _yPosition;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY( EventCategoryMouse | EventCategoryInput)

private:
	float _xPosition;
	float _yPosition;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(const float xOffset, const float yOffset)
		: _xOffset(xOffset), _yOffset(yOffset)
	{ }

	float GetXOffset() const { return _xOffset; }
	float GetYOffset() const { return _yOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << _xOffset << ", " << _yOffset;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float _xOffset;
	float _yOffset;
};

class MouseButtonEvent : public Event
{
public:
	MouseCode GetMouseButton() const { return _button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
protected:
	MouseButtonEvent(const MouseCode button)
		: _button(button)
	{ }

	MouseCode _button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(const MouseCode button)
		: MouseButtonEvent(button) 
	{ }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << _button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(const MouseCode button)
		: MouseButtonEvent(button)
	{}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << _button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};