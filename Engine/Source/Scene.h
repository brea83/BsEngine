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

	void AddRenderable(Model* newRenderable) { _objectsToRender.push_back(newRenderable); }
	void CreateCube();
	void RemoveRenderable(Model* modelToRemove);
	std::vector<Model*>& GetRenderables() { return _objectsToRender; }

	Camera* GetMainCamera() { return _mainCamera; }
private:
	Camera* _mainCamera;
	std::vector<Model*> _objectsToRender; 

	std::vector<GameObject*> _gameObjects;

	friend class SceneHierarchyPanel;
};

