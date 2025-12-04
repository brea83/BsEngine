#pragma once
#include <glm/glm.hpp>
#include <EnTT/entt.hpp>
#include "Editor/EditorCamera.h"

class GameObject;
class MeshComponent;
class Camera;
class CameraComponent;
class Entity;

class Scene
{
public:
	Scene();
	~Scene();
	
	GameObject CreateEmptyGameObject(const std::string& name);
	void AddGameObject(GameObject* gameObject) { m_GameObjects.push_back(gameObject); }
	void RemoveGameObject(GameObject* objectToRemove);
	void RevoveObjectByEntity(entt::entity entityHandle) {}
	int NumGameObjects() { return m_GameObjects.size(); }
	GameObject* GetGameObjectByIndex(int index);
	
	void Initialize();
	void OnUpdate(float deltaTime);

	entt::registry& GetRegistry() { return m_Registry; }
	Entity CreateEntity(const std::string& name = "");

	void AddRenderable(std::shared_ptr<MeshComponent> newRenderable) { m_MeshComponents.push_back(newRenderable); }
	void CreateCube();
	void RemoveRenderable(std::shared_ptr<MeshComponent> modelToRemove);
	std::vector<std::shared_ptr<MeshComponent>>& GetRenderables() { return m_MeshComponents; }

	//void AddCamera(std::shared_ptr<CameraComponent> camera) { m_CameraComponents.push_back(camera); }
	//bool TryRemoveCamera(std::shared_ptr<CameraComponent> cameraToRemove);
	Camera* GetActiveCamera();
	Camera* GetActiveCamera(glm::mat4& viewMatrix);
	entt::entity GetActiveCameraEntity() { return m_ActiveCamera; }
	//Transform& GetActiveCameraTransform();

private:
	entt::registry m_Registry;
	entt::entity m_ActiveCamera;
	entt::entity m_DefaultCamera;
	EditorCamera m_EditorCamera;
	bool m_IsInitiated{ false };

	//std::vector<std::shared_ptr<CameraComponent>> m_CameraComponents;
	std::vector<std::shared_ptr<MeshComponent>> m_MeshComponents;
	std::vector<GameObject*> m_GameObjects;

	friend class SceneHierarchyPanel;
	friend class DetailsViewPanel;
	friend class ImGuiLayer;
};

