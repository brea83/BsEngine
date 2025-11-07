#pragma once
#include "BsPrecompileHeader.h"
#include "Events/Event.h"

class WindowResizedEvent : public Event
{
public:
	WindowResizedEvent(unsigned int width, unsigned int height)
		: _width(width), _height(height)
	{}

	unsigned int GetWidth() const { return _width; }
	unsigned int GetHeight() const { return _height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizedEvent: " << _width << ", " << _height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResized)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	unsigned int _width;
	unsigned int _height;
 };

class WindowClosedEvent : public Event
{
public:
	WindowClosedEvent() = default;
	EVENT_CLASS_TYPE(WindowClosed)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};