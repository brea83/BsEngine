#pragma once
#include "BsPrecompileHeader.h"
#include "Events/Event.h"

namespace Pixie
{
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

	class Scene;
	// I think Scene will need to have been defined in a file you create or handle this event in
	class SceneChangedEvent : public Event
	{
	public:
		SceneChangedEvent(std::shared_ptr<Scene> newScene, bool isPlaymodeSwap = true)
			: m_NewScene(newScene), m_BIsPlaymodeSwap(isPlaymodeSwap) { }

		std::shared_ptr<Scene> GetScene() { return m_NewScene; }
		bool IsPlaymodeSwap() const { return m_BIsPlaymodeSwap; }
		std::string ToString() const override;

		EVENT_CLASS_TYPE(SceneChanged)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		std::shared_ptr<Scene> m_NewScene;
		bool m_BIsPlaymodeSwap{ false };
	};

	class GameStateEvent : public Event
	{
	public:
		GameStateEvent() = default;
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class GameStateChangeRequestEvent : public GameStateEvent
	{
	public:
		GameStateChangeRequestEvent(const std::string_view newState, std::string requestingSource)
			: m_State(newState) { }

		const std::string_view GetState() const { return m_State; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Event: Request Game State Change sent by: " << m_Source << ". Desired new state: " << m_State << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(RequestGameStateChange)
	protected:
		const std::string_view m_State;
		std::string m_Source; // this is mostly for debug should be able to remove this later
	};



	class GameStateEnteredEvent : public GameStateEvent
	{
	public:
		GameStateEnteredEvent(const std::string_view oldState, const std::string_view newState)
			: m_OldState(oldState), m_NewState(newState)
		{}

		const std::string_view EnteredState() const { return m_NewState; }
		const std::string_view PreviousState() const { return m_OldState; }

		EVENT_CLASS_TYPE(GameStateEntered)
	protected:
		const std::string_view m_NewState;
		const std::string_view m_OldState;
	};

	class GameStateExitedEvent : public GameStateEvent
	{
	public:
		GameStateExitedEvent(const std::string_view oldState, const std::string_view newState)
			: m_OldState(oldState), m_NewState(newState)
		{}

		const std::string_view NextState() const { return m_NewState; }
		const std::string_view ExitingState() const { return m_OldState; }

		EVENT_CLASS_TYPE(GameStateExited)
	protected:
		const std::string_view m_NewState;
		const std::string_view m_OldState;
	};

}