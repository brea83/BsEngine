#include "BsPrecompileHeader.h"
#include "CameraManager.h"
#include "EngineContext.h"
#include "Scene/GameObject.h"
#include "Scene/Components/Component.h"
#include "Scene/Components/CameraComponent.h"
#include "Scene/Components/CameraController.h"
#include "Scene/Components/Transform.h"

namespace Pixie
{
    void CameraManager::OnEditorUpdate(float deltaTime)
    {
        GameObject activeCam = GameObject(m_ActiveCamera, m_Scene);
        CameraController* controller = activeCam.TryGetComponent<CameraController>();
        if (!controller) return;
        controller->OnUpdate(deltaTime, activeCam);
    }
    bool CameraManager::OnEvent(Event& event)
    {
        EventDispatcher dispatcher{ event };
        //dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(CameraManager::OnKeyPressed));

        if (!event.Handled)
        {
            CameraController* controllerComponent = m_Scene->GetRegistry().try_get<CameraController>(m_ActiveCamera);
            if (!controllerComponent) return false;
            return controllerComponent->OnEvent(event);
        }

        return false;
    }

    bool CameraManager::OnKeyPressed(KeyPressedEvent& event)
    {
        /*CameraController* controllerComponent = m_Scene->GetRegistry().try_get<CameraController>(m_ActiveCamera);
        if (!controllerComponent) return false;

        Inputs::Keyboard keyCode = (Inputs::Keyboard)event.GetKeyCode();
        if (keyCode == Inputs::Keyboard::Tab)
        {
            if (controllerComponent->GetMoveType() != CameraMoveType::Fly)
            {
                controllerComponent->SetMoveType(CameraMoveType::Fly);
            }
            else
            {
                controllerComponent->SetMoveType(CameraMoveType::WaitingForMouse);
            }
            return true;
        }*/

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

    bool CameraManager::OnCameraAdded(CameraComponent& cameraComponent)
    {
        glm::vec2 viewport = EngineContext::GetEngine()->GetViewportSize();

        cameraComponent.Cam.SetAspectRatio(viewport.x / viewport.y);
        return true;
    }

    bool CameraManager::IsCameraRemovable(entt::entity entityToRemove)
    {
        entt::registry& registry = m_Scene->GetRegistry();
        if (m_EditorCamera == entityToRemove)
        {
            std::cout << "You may not delete the editor camera" << std::endl;
            return false;
        }

        auto view = registry.view<CameraComponent>();
        if (view.size() < 2)
        {
            std::cout << "You may not delete the only camera" << std::endl;
            return false;
        }

        int sceneCameraCount = 0;
        for (auto entityHandle : view)
        {
            HeirarchyComponent* component = registry.try_get<HeirarchyComponent>(entityHandle);
            if(component) sceneCameraCount++;
        }
        
        if (sceneCameraCount < 2)
        {
            std::cout << "You may not delete the only non editor camera" << std::endl;
            return false;
        }
        return true;
    }

    bool CameraManager::OnRemoveCamera(entt::entity entityToRemove)
    {
        if(!IsCameraRemovable(entityToRemove)) return false;
        //GameObject toRemove = GameObject(entityToRemove, m_Scene);

        entt::registry& registry = m_Scene->GetRegistry();

        auto view = registry.view<CameraComponent>();
        if (entityToRemove == m_DefaultCamera)
        {
            for (auto entity : view)
            {
                if (entity != entityToRemove)
                {
                    m_DefaultCamera = GameObject(entity, m_Scene);
                    break;
                }
            }
        }

        if (entityToRemove == m_ActiveCamera)
        {
            m_ActiveCamera = m_DefaultCamera;
            registry.get<CameraComponent>(m_ActiveCamera).IsPrimaryCamera = true;
        }

        return true;
    }

    void CameraManager::OnViewportSizeEvent(float width, float height)
    {
        GameObject activeCam = GameObject(m_ActiveCamera, m_Scene);

        // if active cam is not entt valid we don't want to try and get components for it
        if (!activeCam) return; 

        //cam controller tracks viewport size for move speed stuff
        CameraController& controller = activeCam.GetComponent<CameraController>();
        controller.OnViewportSizeChange(width, height);

        // cam component.cam for rendering viewport size 
        activeCam.GetComponent<CameraComponent>().Cam.SetAspectRatio(width / height);
    }

    void CameraManager::SetActiveCamera(GameObject& gameObject)
    {
        if (m_ActiveCamera == gameObject.GetEnttHandle()) return;
        CameraComponent* nextCamera = gameObject.TryGetComponent<CameraComponent>();
        if (!nextCamera) return;

        if (m_ActiveCamera != entt::null && m_ActiveCamera != entt::tombstone)
        {
            CameraComponent& previousCamera = m_Scene->GetRegistry().get<CameraComponent>(m_ActiveCamera);
            previousCamera.IsPrimaryCamera = false;
        }

        nextCamera->IsPrimaryCamera = true;
        m_ActiveCamera = gameObject;
    }

    Camera* CameraManager::GetActiveCamera()
    {
        entt::registry& registry = m_Scene->GetRegistry();
        CameraComponent* component = registry.try_get<CameraComponent>(m_ActiveCamera);
        if (!component) return nullptr;

        return &(component->Cam);
    }

    Camera* CameraManager::GetActiveCamera(glm::mat4& viewMatrix)
    {
        GameObject activeCam = GameObject(m_ActiveCamera, m_Scene);

        if (!activeCam) return nullptr;
        TransformComponent& transform = activeCam.GetComponent<TransformComponent>();

        glm::vec3 position = transform.GetPosition();
        glm::vec3 rotation = transform.GetRotationEuler(AngleType::Radians);
        glm::vec3 direction;
        direction.x = cos(rotation.x * cos(rotation.y));
        direction.y = sin(rotation.y);
        direction.z = sin(rotation.x) * cos(rotation.y);
        glm::vec3 forward = glm::normalize(direction);

        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        viewMatrix = glm::lookAt(position, position + forward, up);

        CameraComponent& cameraComponent = activeCam.GetComponent<CameraComponent>();
        return &cameraComponent.Cam;
    }

    void CameraManager::SetDefaultCamera(GameObject& gameObject)
    {
        if (m_ActiveCamera == gameObject) return;
        CameraComponent* nextCamera = gameObject.TryGetComponent<CameraComponent>();
        if (!nextCamera) return;

        if (m_ActiveCamera != entt::null && m_ActiveCamera != entt::tombstone)
        {
            CameraComponent& previousCamera = m_Scene->GetRegistry().get<CameraComponent>(m_ActiveCamera);
            previousCamera.IsPrimaryCamera = false;
        }

        nextCamera->IsPrimaryCamera = true;
        m_ActiveCamera = gameObject;
    }

    GameObject CameraManager::GetActiveCameraObject()
    {
        return GameObject(m_ActiveCamera, m_Scene);
    }

    GameObject CameraManager::GetDefaultCamera()
    {
        return GameObject{ m_DefaultCamera, m_Scene };
    }

}