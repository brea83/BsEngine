#pragma once

#include <vector>
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
	void AddGameObject(GameObject* gameObject) { _gameObjects.push_back(gameObject); }
	void RemoveGameObject(GameObject* objectToRemove);
	int NumGameObjects() { return _gameObjects.size(); }
	GameObject* GetGameObjectByIndex(int index);
	

	void AddRenderable(std::shared_ptr<MeshComponent> newRenderable) { _meshComponents.push_back(newRenderable); }
	void CreateCube();
	void RemoveRenderable(std::shared_ptr<MeshComponent> modelToRemove);
	std::vector<std::shared_ptr<MeshComponent>>& GetRenderables() { return _meshComponents; }

	void AddCamera(std::shared_ptr<CameraComponent> camera) { _cameraComponents.push_back(camera); }
	bool TryRemoveCamera(std::shared_ptr<CameraComponent> cameraToRemove);
	std::shared_ptr<Camera> GetActiveCamera() { return _activeCamera; }

private:
	std::shared_ptr<Camera> _activeCamera;
	std::shared_ptr<Camera> _defaultCamera{ nullptr };

	std::vector<std::shared_ptr<CameraComponent>> _cameraComponents;
	std::vector<std::shared_ptr<MeshComponent>> _meshComponents;
	std::vector<GameObject*> _gameObjects;

	friend class SceneHierarchyPanel;
	friend class DetailsViewPanel;
};

