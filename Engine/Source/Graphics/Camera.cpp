#include "BsPrecompileHeader.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

Camera::Camera()
{
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	_right = glm::normalize(glm::cross(worldUp, _forward));
	_up = glm::cross(_forward, _right);
}

void Camera::LookAt(const glm::vec3 & target, const glm::vec3 & newUp)
{
	_target = target;
	_up = newUp;
}

//TODO REFACTOR THIS INTO A MOVEMENT COMPONENT TO BE USED BY SCENE CAM AND OTHER THINGS
	// AND REFACTOR THE SCENE EDIT CAMERA TO BE DIFFERENT FROM THE SCENE'S MAIN GAMEPLAY CAM
bool Camera::HandleMoveInput(int keyCode, float deltaTime)
{
	const float cameraSpeed = 10.0f * deltaTime; // adjust accordingly
	switch (keyCode)
	{
	case GLFW_KEY_W:
		_position += cameraSpeed * _forward;
		return true;
	case GLFW_KEY_S:
			_position -= cameraSpeed * _forward;
		return true;
	case GLFW_KEY_A:
		_position -= glm::normalize(glm::cross(_forward, _up)) * cameraSpeed;
		return true;
	case GLFW_KEY_D:
		_position += glm::normalize(glm::cross(_forward, _up)) * cameraSpeed;
		return true;
	default:
		return false;
	}

}

glm::mat4 Camera::ViewMatrix() const
{
	//cameraPos, cameraPos + cameraFront, cameraUp
	return  glm::lookAt(_position, _position + _forward, _up);
}

glm::mat4 Camera::ProjectionMatrix() const
{
	return glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
}
