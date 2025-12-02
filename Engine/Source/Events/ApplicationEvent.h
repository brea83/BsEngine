#pragma once
#include "BsPrecompileHeader.h"
#include "Events/Event.h"

class WindowResizedEvent : public Event
{
public:
	WindowResizedEvent(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height)
	{}

	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizedEvent: " << m_Width << ", " << m_Height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResized)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	unsigned int m_Width;
	unsigned int m_Height;
 };

class WindowClosedEvent : public Event
{
public:
	WindowClosedEvent() = default;
	EVENT_CLASS_TYPE(WindowClosed)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};