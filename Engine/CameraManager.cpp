#include "BsPrecompileHeader.h"
#include "EngineContext.h"
#include "CameraManager.h"
#include "Scene/Components/CameraComponent.h"

bool CameraManager::OnEvent(Event& event)
{
    return false;
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

bool CameraManager::OnCameraAdded(Entity& entity)
{
    return false;
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
