#pragma once
#include <glm/glm.hpp>
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvents.h"
#include "Events/KeyboardEvents.h"
#include <EnTT/entt.hpp>

namespace Pixie
{
	class Transform;
	class GameObject;

	class CameraController
	{
	public:
		CameraController() = default;
		CameraController(entt::entity entity) : m_CameraEntity(entity) {}

		void OnUpdate(float deltaTime, GameObject& gameObject);

		bool OnEvent(Event& event);
		bool HandleKeyInput(Transform* transform, Inputs::Keyboard keyCode, float deltaTime);
		bool HandleMouseLook(Transform* transform, float xOffset, float yOffset, float deltaTime);

	private:
		entt::entity m_CameraEntity{ entt::null };

		bool m_Rotation{ false };
		glm::vec3 m_CameraPosition{ 0.0f };

		/*float m_Yaw{ 0.0f };
		float m_Pitch{ 0.0f };*/
		// rotation speed in degrees
		float m_RotationSpeed{ 10.0f };
		glm::vec2 m_prevMousePosition{ 0.0f };
		glm::vec2 m_MouseDelta{ 0.0f };
		bool m_FirstMouseFrame{ true };

		glm::vec3 m_TranslationDirection{ 0.0f };
		float m_TranslationSpeed{ 10.0f };

		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizedEvent& event);
	};

}