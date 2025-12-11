#include "BsPrecompileHeader.h"
#include "EngineContext.h"
#include "CameraManager.h"
#include "Scene/Components/CameraComponent.h"
#include "Graphics/CameraController.h"


bool CameraManager::OnEvent(Event& event)
{
    EventDispatcher dispatcher{ event };
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(CameraManager::OnKeyPressed));
    
    if (!event.Handled && m_EditorFlyMode)
    {
        CameraController* controllerComponent = m_ActiveCamera.TryGetComponent<CameraController>();
        if (!controllerComponent) return false;
        return controllerComponent->OnEvent(event);
    }

    return false;
}

bool CameraManager::OnKeyPressed(KeyPressedEvent& event)
{
    Inputs::Keyboard keyCode = (Inputs::Keyboard)event.GetKeyCode();
    if (keyCode == Inputs::Keyboard::Tab)
    {
        m_EditorFlyMode = !m_EditorFlyMode;
        return true;
    }
}
void CameraManager::OnBeginPlayMode()
{
    if (EngineContext::GetEngine()->IsEditorEnabled())
    {
        //set active camera to main scene camera
    }
}

//void CameraManager::OnPlayModeUpdate()
//{}

void CameraManager::OnEndPlayMode()
{
    if (EngineContext::GetEngine()->IsEditorEnabled())
    {
        m_ActiveCamera = m_EditorCamera;
    }
}

void CameraManager::OnEditorUpdate(float deltaTime)
{
    if (m_EditorFlyMode)
    {
        CameraController* controller = m_ActiveCamera.TryGetComponent<CameraController>();
        if (controller)
        {
            controller->OnUpdate(deltaTime, m_ActiveCamera);
        }
    }
}

bool CameraManager::OnCameraAdded(CameraComponent& cameraComponent)
{
    glm::vec2 viewport = EngineContext::GetEngine()->GetViewportSize();

    cameraComponent.Cam.SetAspectRatio(viewport.x / viewport.y);
    return true;
}

bool CameraManager::IsCameraRemovable(Entity& entity)
{
    return false;
}

bool CameraManager::TryRemoveCameraComponent(Entity& entity)
{
    return false;
}

void CameraManager::SetActiveCamera(GameObject& gameObject)
{
    m_ActiveCamera = gameObject;
}

Camera* CameraManager::GetActiveCamera()
{
    return &m_ActiveCamera.GetComponent<CameraComponent>().Cam;
}
