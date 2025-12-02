#pragma once
#include "Graphics/Camera.h"
class EditorCamera : public Camera
{
public:
	EditorCamera();

	// Inherited via Camera
	//void LookAt(const glm::vec3& target, const glm::vec3& up) override;
	bool HandleMoveWasd(int keyCode, float deltaTime) override;
	bool HandleLookMouse(float xOffset, float yOffset, float deltaTime) override;
	glm::mat4 ViewMatrix() const override;

protected:
	glm::vec3 m_Position{ 0.0f, 0.0f, 10.0f };
	glm::vec3 m_Target{ 0.0f };
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_Forward;

	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;

	void UpdateCameraVectors();
};

