#include "BsPrecompileHeader.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Scene/GameObject.h"
#include "Graphics/Primitives/Transform.h"

Camera::Camera()
{}

bool Camera::Zoom(float amount)
{
	_fov -= amount;

	if (_fov < 1.0f) _fov = 1.0f;

	if (_fov > 90.0f) _fov = 90.0f;
	return true;
}

glm::mat4 Camera::ProjectionMatrix() const
{
	return glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
}