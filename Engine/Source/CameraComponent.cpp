#include "BsPrecompileHeader.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "Graphics/Primitives/Transform.h"
#include <GLFW/glfw3.h>

void CameraComponent::Initialize()
{}

void CameraComponent::CleanUp()
{}

std::shared_ptr<Component> CameraComponent::Clone()
{
    return std::shared_ptr<Component>();
}

void CameraComponent::SetParentObject(GameObject* newParent)
{}

void CameraComponent::OnUpdate()
{}

//TODO REFACTOR THIS INTO A MOVEMENT COMPONENT TO BE USED BY SCENE CAM AND OTHER THINGS
	// AND REFACTOR THE SCENE EDIT CAMERA TO BE DIFFERENT FROM THE SCENE'S MAIN GAMEPLAY CAM
bool CameraComponent::HandleMoveWasd(int keyCode, float deltaTime)
{
	float velocity = _cameraSpeed * deltaTime; // adjust accordingly
	std::shared_ptr<Transform> transform = _parentObject->GetTransform();
	glm::vec3 currentPosition = transform->GetPosition();
	switch (keyCode)
	{
	case GLFW_KEY_W:
		transform->SetPosition(currentPosition + (velocity * transform->Forward()));
		return true;
	case GLFW_KEY_S:
		transform->SetPosition(currentPosition + (velocity * transform->Forward() * -1.0f));
		return true;
	case GLFW_KEY_A:
		transform->SetPosition(currentPosition + (velocity * transform->Left()));
		return true;
	case GLFW_KEY_D:
		transform->SetPosition(currentPosition + (velocity * transform->Right()));
		return true;
	default:
		return false;
	}

}

bool CameraComponent::HandleLookMouse(float xOffset, float yOffset, float deltaTime)
{
	const float sensitivity = _mouseLookSesitivity * deltaTime;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	std::shared_ptr<Transform> transform = _parentObject->GetTransform();
	glm::vec3 rotation = transform->GetRotationEuler(AngleType::Degrees);
	transform->SetRotationEuler(glm::vec3(rotation.x + xOffset, rotation.y + yOffset, rotation.z), AngleType::Degrees);

	return true;
}

glm::mat4 CameraComponent::ViewMatrix() const
{
	std::shared_ptr<Transform> transform = _parentObject->GetTransform();
	glm::vec3 position = transform->GetPosition();
	return  glm::lookAt(position, position + transform->Forward(), transform->Up());
}
