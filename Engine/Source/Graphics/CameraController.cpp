#include "BsPrecompileHeader.h"
#include "CameraController.h"

#include "Input/WindowsInput.h"
#include "Events/KeyCodes.h"
#include "Events/MouseCodes.h"

#define GLFW_INCLUDE_NONE
#include "EngineContext.h"
#include "Scene/Scene.h"
#include "Graphics/Primitives/Transform.h"
#include "Scene/Components/CameraComponent.h"

#define BIND_EVENT_FUNCTION(x) [this](auto&&... args) -> decltype(auto){ return this->x(std::forward<decltype(args)>(args)...);}

void CameraController::OnUpdate(float deltaTime)
{ }

//entt::registry& CameraController::GetRegistry()
//{
//	return EngineContext::GetEngine()->GetScene()->GetRegistry();
//}

bool CameraController::OnEvent(Transform* transform, Event& event, float deltaTime)
{
	EventDispatcher dispatcher{ event };

	//dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(EngineContext::OnKeyPressedEvent));

	//dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(EngineContext::OnMouseButtonPressedEvent));
	return dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(CameraController::OnMouseScrolled));
	//dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(EngineContext::OnMouseMoved));
}

bool CameraController::HandleKeyInput(Transform* transform, Inputs::Keyboard keyCode, float deltaTime)
{
	float velocity = m_TranslationSpeed * deltaTime; // adjust accordingly

	glm::vec3 currentPosition = transform->GetPosition();
	switch (keyCode)
	{
	case Inputs::Keyboard::W:
		transform->SetPosition(currentPosition + (velocity * transform->Forward()));
		return true;
	case Inputs::Keyboard::S:
		transform->SetPosition(currentPosition + (velocity * transform->Forward() * -1.0f));
		return true;
	case Inputs::Keyboard::A:
		transform->SetPosition(currentPosition + (velocity * transform->Left()));
		return true;
	case Inputs::Keyboard::D:
		transform->SetPosition(currentPosition + (velocity * transform->Right()));
		return true;
	default:
		return false;
	}
}

//void CameraController::OnResize(float width, float height)
//{
//	CameraComponent* cameraComponent = GetRegistry().try_get<CameraComponent>(m_CameraEntity);
//
//	if (cameraComponent == nullptr) return;
//
//	cameraComponent->Cam.SetAspectRatio(width / height);
//
//}

bool CameraController::OnKeyPressed( KeyPressedEvent& event)
{
	return false;
}

bool CameraController::OnMouseButtonPressed( MouseButtonPressedEvent& evente)
{
	return false;
}

bool CameraController::HandleMouseLook(Transform* transform, float xOffset, float yOffset, float deltaTime)
{
	float sensitivity = m_RotationSpeed * deltaTime;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	glm::vec3 rotation = transform->GetRotationEuler(AngleType::Degrees);
	transform->SetRotationEuler(glm::vec3(rotation.x + xOffset, rotation.y + yOffset, rotation.z), AngleType::Degrees);
	
	return true;
}

bool CameraController::OnMouseScrolled(MouseScrolledEvent& event)
{
	float delta = event.GetYOffset() * 0.1f;
	//MouseZoom(delta);
	//UpdateView();
	return false;
}

bool CameraController::OnWindowResized(WindowResizedEvent& event)
{
	return false;
}