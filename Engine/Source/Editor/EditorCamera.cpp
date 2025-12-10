#include "BsPrecompileHeader.h"
#include "EditorCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace Pixie
{
	EditorCamera::EditorCamera()
	{
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_Forward = glm::vec3(0.0f, 0.0f, -1.0f);
		m_Right = glm::normalize(glm::cross(worldUp, m_Forward));
		m_Up = glm::cross(m_Forward, m_Right);
	}

	//void EditorCamera::LookAt(const glm::vec3& target, const glm::vec3& up)
	//{
	//	m_Target = target;
	//	m_Up = up;
	//}

	bool EditorCamera::HandleMoveWasd(int keyCode, float deltaTime)
	{
		float velocity = m_CameraSpeed * deltaTime; // adjust accordingly

		switch (keyCode)
		{
		case GLFW_KEY_W:
			m_Position += velocity * m_Forward;
			return true;
		case GLFW_KEY_S:
			m_Position -= velocity * m_Forward;
			return true;
		case GLFW_KEY_A:
			m_Position -= glm::normalize(glm::cross(m_Forward, m_Up)) * velocity;
			return true;
		case GLFW_KEY_D:
			m_Position += glm::normalize(glm::cross(m_Forward, m_Up)) * velocity;
			return true;
		default:
			return false;
		}
	}

	bool EditorCamera::HandleLookMouse(float xOffset, float yOffset, float deltaTime)
	{
		const float sensitivity = m_MouseLookSesitivity * deltaTime;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		m_Yaw += xOffset;
		m_Pitch += yOffset;

		UpdateCameraVectors();

		return true;
	}

	glm::mat4 EditorCamera::ViewMatrix() const
	{
		return  glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}

	void EditorCamera::UpdateCameraVectors()
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Yaw) * cos(glm::radians(m_Pitch)));
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Forward = glm::normalize(direction);

		m_Right = glm::normalize(glm::cross(m_Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
	}
}