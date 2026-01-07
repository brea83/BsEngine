#include "BsPrecompileHeader.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Pixie
{
	Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip)
		:   m_Fov(fov), m_AspectRatio(aspectRatio), m_Near(nearClip), m_Far(farClip)
	{}

	bool Camera::Zoom(float amount)
	{
		m_ZoomLevel -= amount;

		//m_Fov -= amount;

		if (m_ZoomLevel < 0.03f)
		{
			m_ZoomLevel = 0.03f;
		}
		else if (m_ZoomLevel > 2.0f)
		{
			m_ZoomLevel = 2.0f;
		}
		/*if (m_Fov < 1.0f) m_Fov = 1.0f;

		if (m_Fov > 90.0f) m_Fov = 90.0f;*/
		return true;
	}

	glm::mat4 Camera::ProjectionMatrix() const
	{
		return glm::perspective(glm::radians(m_Fov * m_ZoomLevel), m_AspectRatio, m_Near, m_Far);
	}
	glm::mat4 Camera::OrthoProjection() const
	{
		return glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_Near, m_Far);
	}
}