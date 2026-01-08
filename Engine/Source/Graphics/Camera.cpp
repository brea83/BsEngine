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
		if (m_IsOrthographic)
			return OrthoProjection();

		return PerspectiveProjection();
	}
	glm::mat4 Camera::PerspectiveProjection() const
	{
		float nonZeroZoom = glm::max(0.001f, m_ZoomLevel);
		float aspectRatio = m_LockAspectRatio ? m_ManualRatio : m_AspectRatio;
		return glm::perspective(glm::radians(m_Fov * nonZeroZoom), aspectRatio, m_Near, m_Far);
	}
	glm::mat4 Camera::OrthoProjection() const
	{
		float nonZeroZoom = glm::max(0.001f, m_ZoomLevel);
		float aspectRatio = m_LockAspectRatio ? m_ManualRatio : m_AspectRatio;
		return glm::ortho(-aspectRatio * nonZeroZoom, aspectRatio * nonZeroZoom, -nonZeroZoom, nonZeroZoom, m_Near, m_Far);
	}
}