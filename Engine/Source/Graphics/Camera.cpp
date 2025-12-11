#include "BsPrecompileHeader.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Pixie
{
	Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip)
		:   m_Fov(fov), m_AspectRatio(aspectRatio), m_Near(nearClip), m_Far(farClip)
	{}

	//////TODO REFACTOR THIS INTO A MOVEMENT COMPONENT TO BE USED BY SCENE CAM AND OTHER THINGS
	////	// AND REFACTOR THE SCENE EDIT CAMERA TO BE DIFFERENT FROM THE SCENE'S MAIN GAMEPLAY CAM
	//bool Camera::HandleMoveWasd(int keyCode, float deltaTime)
	//{
	//	float velocity = m_CameraSpeed * deltaTime; // adjust accordingly
	//	std::shared_ptr<Transform> transform = m_ParentObject->GetTransform();
	//	glm::vec3 currentPosition = transform->GetPosition();
	//	switch (keyCode)
	//	{
	//	case GLFW_KEY_W:
	//		transform->SetPosition(currentPosition + (velocity * transform->Forward()));
	//		return true;
	//	case GLFW_KEY_S:
	//		transform->SetPosition(currentPosition + (velocity * transform->Forward() * -1.0f));
	//		return true;
	//	case GLFW_KEY_A:
	//		transform->SetPosition(currentPosition + (velocity * transform->Left()));
	//		return true;
	//	case GLFW_KEY_D:
	//		transform->SetPosition(currentPosition + (velocity * transform->Right()));
	//		return true;
	//	default:
	//		return false;
	//	}
	//
	//}
	//
	//bool Camera::HandleLookMouse(float xOffset, float yOffset, float deltaTime)
	//{
	//	const float sensitivity = m_MouseLookSesitivity * deltaTime;
	//	xOffset *= sensitivity;
	//	yOffset *= sensitivity;
	//
	//	std::shared_ptr<Transform> transform = m_ParentObject->GetTransform();
	//	glm::vec3 rotation = transform->GetRotationEuler(AngleType::Degrees);
	//	transform->SetRotationEuler(glm::vec3(rotation.x + xOffset, rotation.y + yOffset, rotation.z), AngleType::Degrees);
	//
	//	return true;
	//}
	//
	//glm::mat4 Camera::ViewMatrix() const
	//{
	//	std::shared_ptr<Transform> transform = m_ParentObject->GetTransform();
	//	glm::vec3 position = transform->GetPosition();
	//	return  glm::lookAt(position, position + transform->Forward(), transform->Up());
	//}

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
}