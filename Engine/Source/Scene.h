#pragma once

#include <vector>
//#include <memory>

class GameObject;
class Model;
class Camera;

class Scene
{
public:
	Scene();
	~Scene();
	
	void CreateEmptyGameObject();
	void AddGameObject(GameObject* gameObject) { _gameObjects.push_back(gameObject); }
	void RemoveGameObject(GameObject* objectToRemove);

	void AddRenderable(std::shared_ptr<Model> newRenderable) { _objectsToRender.push_back(newRenderable); }
	void CreateCube();
	void RemoveRenderable(std::shared_ptr<Model> modelToRemove);
	std::vector<std::shared_ptr<Model>>& GetRenderables() { return _objectsToRender; }

	Camera* GetMainCamera() { return _mainCamera; }
private:
	Camera* _mainCamera;
	std::vector<std::shared_ptr<Model>> _objectsToRender;

	std::vector<GameObject*> _gameObjects;

	friend class SceneHierarchyPanel;
	friend class DetailsViewPanel;
};

