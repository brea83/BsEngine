#pragma once

#include <vector>
#include <EnTT/entt.hpp>
//#include <memory>

class GameObject;
class MeshComponent;
class Camera;
class CameraComponent;
class EditorCamera;

class Scene
{
public:
	Scene();
	~Scene();
	
	GameObject* CreateEmptyGameObject();
	void AddGameObject(GameObject* gameObject) { m_GameObjects.push_back(gameObject); }
	void RemoveGameObject(GameObject* objectToRemove);
	int NumGameObjects() { return m_GameObjects.size(); }
	GameObject* GetGameObjectByIndex(int index);
	
	void Initialize();

	entt::registry& GetRegistry() { return m_Registry; }
	entt::entity CreateEntity();

	void AddRenderable(std::shared_ptr<MeshComponent> newRenderable) { m_MeshComponents.push_back(newRenderable); }
	void CreateCube();
	void RemoveRenderable(std::shared_ptr<MeshComponent> modelToRemove);
	std::vector<std::shared_ptr<MeshComponent>>& GetRenderables() { return m_MeshComponents; }

	void AddCamera(std::shared_ptr<CameraComponent> camera) { m_CameraComponents.push_back(camera); }
	bool TryRemoveCamera(std::shared_ptr<CameraComponent> cameraToRemove);
	std::shared_ptr<Camera> GetActiveCamera() { return m_ActiveCamera; }

private:
	entt::registry m_Registry;
	std::shared_ptr<Camera> m_ActiveCamera;
	std::shared_ptr<Camera> m_DefaultCamera{ nullptr };
	bool m_IsInitiated{ false };

	std::vector<std::shared_ptr<CameraComponent>> m_CameraComponents;
	std::vector<std::shared_ptr<MeshComponent>> m_MeshComponents;
	std::vector<GameObject*> m_GameObjects;

	friend class SceneHierarchyPanel;
	friend class DetailsViewPanel;
};

