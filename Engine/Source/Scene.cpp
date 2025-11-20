#include "BsPrecompileHeader.h"
#include "Scene.h"
#include "GameObject.h"
#include "Graphics/Model.h"
#include "Graphics/Primitives/Cube.h"
#include "Graphics/Camera.h"

Scene::Scene()
{ 
	_mainCamera = new Camera();
	//NextUID = 0;
}

Scene::~Scene()
{
	// unload scene objects that do not persist across scenes
	if (!_gameObjects.empty())
	{
		for (GameObject* gameObject : _gameObjects)
		{
			delete gameObject;
		}
	}
}

void Scene::CreateEmptyGameObject()
{
	_gameObjects.emplace_back(new GameObject());
}

void Scene::RemoveGameObject(GameObject* objectToRemove)
{
	if (objectToRemove == nullptr) return;

	auto itterator = std::find(_gameObjects.begin(), _gameObjects.end(), objectToRemove);
	if (itterator != _gameObjects.end())
	{

		_gameObjects.erase(itterator);
		delete objectToRemove;

		return;
	}
	std::cout << "Tried to remove: " << objectToRemove << ", from Scene, but could not find it" << std::endl;
}

void Scene::CreateCube()
{
	//_objectsToRender.emplace_back(new Cube());
}

void Scene::RemoveRenderable(std::shared_ptr<Model> modelToRemove)
{
	auto foundItterator = std::find(_objectsToRender.begin(), _objectsToRender.end(), modelToRemove);

	if (foundItterator != _objectsToRender.end())
	{
		std::cout << "FOUND OBJECT: " << modelToRemove->Name << std::endl;
		_objectsToRender.erase(foundItterator);
	}
	else
	{
		std::cout << modelToRemove->Name << ", NOT FOUND" << std::endl;
	}
}
