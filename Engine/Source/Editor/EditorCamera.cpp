#include "BsPrecompileHeader.h"
#include "EditorCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

EditorCamera::EditorCamera()
{
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	_right = glm::normalize(glm::cross(worldUp, _forward));
	_up = glm::cross(_forward, _right);
}

//void EditorCamera::LookAt(const glm::vec3& target, const glm::vec3& up)
//{
//	_target = target;
//	_up = up;
//}

bool EditorCamera::HandleMoveWasd(int keyCode, float deltaTime)
{
	float velocity = _cameraSpeed * deltaTime; // adjust accordingly

	switch (keyCode)
	{
	case GLFW_KEY_W:
		_position += velocity * _forward;
		return true;
	case GLFW_KEY_S:
		_position -= velocity * _forward;
		return true;
	case GLFW_KEY_A:
		_position -= glm::normalize(glm::cross(_forward, _up)) * velocity;
		return true;
	case GLFW_KEY_D:
		_position += glm::normalize(glm::cross(_forward, _up)) * velocity;
		return true;
	default:
		return false;
	}
}

bool EditorCamera::HandleLookMouse(float xOffset, float yOffset, float deltaTime)
{
	const float sensitivity = _mouseLookSesitivity * deltaTime;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	_yaw += xOffset;
	_pitch += yOffset;

	UpdateCameraVectors();

	return true;
}

glm::mat4 EditorCamera::ViewMatrix() const
{
	return  glm::lookAt(_position, _position + _forward, _up);
}

void EditorCamera::UpdateCameraVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(_yaw) * cos(glm::radians(_pitch)));
	direction.y = sin(glm::radians(_pitch));
	direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_forward = glm::normalize(direction);

	_right = glm::normalize(glm::cross(_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	_up = glm::normalize(glm::cross(_right, _forward));
}