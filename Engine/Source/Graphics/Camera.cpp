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
	m_Fov -= amount;

	if (m_Fov < 1.0f) m_Fov = 1.0f;

	if (m_Fov > 90.0f) m_Fov = 90.0f;
	return true;
}

glm::mat4 Camera::ProjectionMatrix() const
{
	return glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);
}