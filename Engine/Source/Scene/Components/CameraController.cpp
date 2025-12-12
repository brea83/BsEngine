#include "BsPrecompileHeader.h"
#include "Core.h"
#include "CameraController.h"

#include "Input/WindowsInput.h"
#include "Events/KeyCodes.h"
#include "Events/MouseCodes.h"

#define GLFW_INCLUDE_NONE
#include "EngineContext.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Scene/Components/Transform.h"
#include "Scene/Components/CameraComponent.h"

//#define BIND_EVENT_FUNCTION(x) [this](auto&&... args) -> decltype(auto){ return this->x(std::forward<decltype(args)>(args)...);}

namespace Pixie
{
	void CameraController::OnUpdate(float deltaTime, GameObject& gameObject)
	{
		std::cout << "Camera Controller Component update" << std::endl;
		TransformComponent& transform = gameObject.GetTransform();

		float sensitivity = m_RotationSpeed * deltaTime;
		glm::vec2 offset = m_MouseDelta * sensitivity;

		glm::vec3 rotation = transform.GetRotationEuler(AngleType::Degrees);
		transform.SetRotationEuler(glm::vec3(rotation.x + offset.x, rotation.y + offset.y, rotation.z), AngleType::Degrees);

		float velocity = m_TranslationSpeed * deltaTime; // adjust accordingly
		glm::vec3 currentPosition = transform.GetPosition();

		glm::vec3 direction = glm::normalize(m_TranslationDirection);

		transform.SetPosition(currentPosition + (velocity * direction));
	}

	bool CameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher{ event };

		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(CameraController::OnKeyPressed));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(CameraController::OnMouseMoved));

		return event.Handled;
		//dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(EngineContext::OnMouseButtonPressedEvent));
		//dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(EngineContext::OnMouseMoved));
	}

	bool CameraController::HandleKeyInput(TransformComponent* transform, Inputs::Keyboard keyCode, float deltaTime)
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


	bool CameraController::OnKeyPressed(KeyPressedEvent& event)
	{
		Inputs::Keyboard keyCode = (Inputs::Keyboard)event.GetKeyCode();

		switch (keyCode)
		{
		case Inputs::Keyboard::W:
			m_TranslationDirection.y = 1;
			return true;
		case Inputs::Keyboard::S:
			m_TranslationDirection.y = -1;
			return true;
		case Inputs::Keyboard::A:
			m_TranslationDirection.y = -1;
			return true;
		case Inputs::Keyboard::D:
			m_TranslationDirection.y = 1;
			return true;
		default:
			return false;
		}
		return false;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& event)
	{
		float x = event.GetX();
		float y = event.GetY();
		glm::vec2 currentMouse{ x, y };

		if (m_FirstMouseFrame)
		{
			m_prevMousePosition = currentMouse;
			m_MouseDelta = glm::vec2{ 0.0f };
			return true;
		}

		m_MouseDelta = m_prevMousePosition - currentMouse;
		m_prevMousePosition = currentMouse;

		return true;
	}

	bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		return false;
	}

	bool CameraController::HandleMouseLook(TransformComponent* transform, float xOffset, float yOffset, float deltaTime)
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
}