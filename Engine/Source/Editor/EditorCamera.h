#pragma once
#include "Graphics/Camera.h"
#include "Events/MouseEvents.h"

namespace Pixie
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(float deltaTime);
		void OnEvent(Event& event);

		float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		// Inherited via Camera
		//void LookAt(const glm::vec3& target, const glm::vec3& up) override;
		bool HandleMoveWasd(int keyCode, float deltaTime) override;
		bool HandleLookMouse(float xOffset, float yOffset, float deltaTime) override;
		glm::mat4 ViewMatrix() const;

	protected:
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position{ 0.0f, 0.0f, 10.0f };
		glm::vec3 m_FocalPoint{ 0.0f, 0.0f, 0.0f };
		//glm::vec3 m_Target{ 0.0f };
		glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right{ -1.0f, 0.0f, 0.0f };
		glm::vec3 m_Forward{ 0.0f, 0.0f, -1.0f };

		float m_Yaw{ -90.0f };
		float m_Pitch{ 0.0f };

		float m_Distance{ 10.0f };

		float m_ViewportWidth{ 1280.0f };
		float m_ViewportHeight{ 720.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		void UpdateCameraVectors();
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScrolled(MouseScrolledEvent& event);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		glm::vec2 PanSpeed() const;
		float RotationSpeed() const { return 0.8f; }


		float ZoomSpeed() const;
	};
}
