#include "BsPrecompileHeader.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	_forward = glm::normalize(_position - _target);
	_right = glm::normalize(glm::cross(worldUp, _forward));
	_up = glm::cross(_forward, _right);
}

void Camera::LookAt(const glm::vec3 & target, const glm::vec3 & newUp)
{
	_target = target;
	_up = newUp;
}

glm::mat4 Camera::ViewMatrix() const
{
	return  glm::lookAt(_position, _target, _up);
}

glm::mat4 Camera::ProjectionMatrix() const
{
	return glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
}
