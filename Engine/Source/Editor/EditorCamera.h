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
	glm::vec3 _position{ 0.0f, 0.0f, 10.0f };
	glm::vec3 _target{ 0.0f };
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _forward;

	float _yaw = -90.0f;
	float _pitch = 0.0f;

	void UpdateCameraVectors();
};

