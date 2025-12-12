#include <Pixie.h>
#include <Source/EntryPoint.h>
#include "Editor/EditorLayer.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"

using namespace Pixie;
class SandboxApp : public Pixie::EngineContext
{
public:
	SandboxApp() : EngineContext()
	{
		SetImGuiLayer(new Pixie::EditorLayer());
	}
	~SandboxApp() {}

protected:
	
	virtual bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event) override;
	virtual bool OnMouseScrolled(MouseScrolledEvent& event) override;
	virtual bool OnMouseMoved(MouseMovedEvent& event) override;
	virtual bool OnKeyPressedEvent(KeyPressedEvent& event) override;
};

Pixie::EngineContext* Pixie::CreateApplication()
{

	return new SandboxApp();
}

bool SandboxApp::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
{
	return false;
}

bool SandboxApp::OnMouseScrolled(MouseScrolledEvent& event)
{
	float yOffset = event.GetYOffset();
	if (m_CamFlyMode)
	{
		return m_ActiveScene->GetActiveCamera()->Zoom(yOffset);
	}
	return false;
}

bool SandboxApp::OnMouseMoved(MouseMovedEvent& event)
{
	if (m_CamFlyMode)
	{
		float xPosition = event.GetX();
		float yPosition = event.GetY();

		if (m_FirstMouse)
		{
			m_PrevMouseX = xPosition;
			m_PrevMouseY = yPosition;
			m_FirstMouse = false;
		}

		float xOffset = xPosition - m_PrevMouseX;
		float yOffset = yPosition - m_PrevMouseY;

		m_PrevMouseX = xPosition;
		m_PrevMouseY = yPosition;

		entt::registry& registry = m_ActiveScene->GetRegistry();
		entt::entity activeCamEntity = m_ActiveScene->GetActiveCameraGameObject();

		CameraController* cameraComponent = registry.try_get<CameraController>(activeCamEntity);
		Transform* cameraTransform = registry.try_get<Transform>(activeCamEntity);

		if (cameraComponent == nullptr || cameraTransform == nullptr) return false;

		return cameraComponent->HandleMouseLook(cameraTransform, xOffset, yOffset, m_DeltaTime);
	}
	return false;
}

bool SandboxApp::OnKeyPressedEvent(KeyPressedEvent& event)
{
	Inputs::Keyboard keyCode = (Inputs::Keyboard)event.GetKeyCode();


	/*if (Inputs::KeyboardNames.find(key) != Inputs::KeyboardNames.end())
	{
		std::cout << event.ToString() << " named: " << Inputs::KeyboardNames.at(key) << std::endl;
	}
	else
	{
		std::cout << event.ToString() << ", and key not in lookup tables " << std::endl;
	}*/

	// TODO: refactor this into a proper input system
	if (m_CamFlyMode &&
		(keyCode == Inputs::Keyboard::W
			|| keyCode == Inputs::Keyboard::S
			|| keyCode == Inputs::Keyboard::A
			|| keyCode == Inputs::Keyboard::D))
	{
		entt::registry& registry = m_ActiveScene->GetRegistry();
		GameObject activeCamObject = m_ActiveScene->GetActiveCameraGameObject();

		CameraController* cameraComponent = registry.try_get<CameraController>(activeCamObject);
		Transform* cameraTransform = registry.try_get<Transform>(activeCamObject);

		if (cameraComponent == nullptr || cameraTransform == nullptr) return false;

		return cameraComponent->HandleKeyInput(cameraTransform, keyCode, m_DeltaTime);
	}

	if (keyCode == Inputs::Keyboard::Tab)
	{
		//TODO: SERIOUSLY NEED A BETTER WAY THAN THESE HARDCODED THINGS
		ToggleCamFlyMode();
		return false;
	}

	m_ImGuiLayer->OnEvent(event);
	return false;
}
