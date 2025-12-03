#pragma once
#include <glm/glm.hpp>
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvents.h"
#include "Events/KeyboardEvents.h"
#include <EnTT/entt.hpp>

class Transform;

class CameraController
{
public:
	CameraController() = default;
	CameraController(entt::entity entity) : m_CameraEntity(entity) {}

	void OnUpdate(float deltaTime);

	bool OnEvent(Transform* transform, Event& event, float deltaTime);
	bool HandleKeyInput(Transform* transform, int keyCode, float deltaTime);
	bool HandleMouseLook(Transform* transform, float xOffset, float yOffset, float deltaTime);

	//void OnResize(float width, float height);

	//Camera& GetCamera() { return m_Camera; }
	//const Camera& GetCamera() const { return m_Camera; }

	float GetZoomLevel() const { return m_ZoomLevel; }
	void SetZoomLevel(float value) { m_ZoomLevel = value; }



private:
	entt::entity m_CameraEntity{ entt::null };
	float m_AspectRatio{ 1280.0f / 720.0f };
	float m_ZoomLevel{ 1.0f };

	bool m_Rotation{ false };
	glm::vec3 m_CameraPosition{ 0.0f };

	float m_Yaw{ 0.0f };
	float m_Pitch{ 0.0f };

	float m_TranslationSpeed{ 10.0f };
	// rotation speed in degrees
	float m_RotationSpeed{ 10.0f };

	entt::registry& GetRegistry();

	bool OnKeyPressed(KeyPressedEvent& event);
	bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
	bool OnMouseScrolled(MouseScrolledEvent& event);
	bool OnWindowResized(WindowResizedEvent& event);
};

