#include "BsPrecompileHeader.h"
#include "Scene.h"
#include "GameObject.h"
#include "Scene/Components/MeshComponent.h"
#include "Graphics/Primitives/Cube.h"
#include "Graphics/Primitives/Transform.h"
#include "Editor/EditorCamera.h"
#include "Components/CameraComponent.h"
#include "EngineContext.h"

Scene::Scene()
{ 

	GameObject* cameraObject = CreateEmptyGameObject();
	cameraObject->Name = "Main Camera";
	std::shared_ptr<Transform> transform = cameraObject->GetTransform();
	transform->SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	transform->SetRotationEuler(glm::vec3(90.0f, 0.0f, 0.0f), AngleType::Degrees);
	
	_activeCamera = cameraObject->AddComponent<CameraComponent>();

	_activeCamera->BIsSceneViewCam = true;
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

GameObject* Scene::CreateEmptyGameObject()
{
	GameObject* newObject = new GameObject();
	_gameObjects.push_back(newObject);
	return newObject;
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

GameObject* Scene::GetGameObjectByIndex(int index)
{
	
	if (index < 0 || NumGameObjects() <= index)
	{
		return nullptr;
	}

	return _gameObjects[index];
	
}

entt::entity Scene::CreateEntity()
{
	EngineContext* engine = EngineContext::GetEngine();
	return engine->GetRegistry().create();
}

void Scene::CreateCube()
{
	//_meshComponents.emplace_back(new Cube());
	GameObject* object = new GameObject("Cube");
	AddGameObject(object);
	object->AddComponent<MeshComponent, PrimitiveMeshType>(PrimitiveMeshType::Cube);
}

void Scene::RemoveRenderable(std::shared_ptr<MeshComponent> modelToRemove)
{
	auto foundItterator = std::find(_meshComponents.begin(), _meshComponents.end(), modelToRemove);

	if (foundItterator != _meshComponents.end())
	{
		std::cout << "FOUND OBJECT: " << modelToRemove->Name() << std::endl;
		_meshComponents.erase(foundItterator);
	}
	else
	{
		std::cout << modelToRemove->Name() << ", NOT FOUND" << std::endl;
	}
}

bool Scene::TryRemoveCamera(std::shared_ptr<CameraComponent> cameraToRemove)
{
	if (_cameraComponents.size() < 2)
	{
		std::cout << "Error: tried to delete last camera component, but scene requires minimum of one camera" << std::endl;
		return false;
	}

	auto foundItterator = std::find(_cameraComponents.begin(), _cameraComponents.end(), cameraToRemove);

	if (foundItterator == _cameraComponents.end())
	{
		std::cout << cameraToRemove->Name() << ", NOT FOUND, can't remove it" << std::endl;
		return false;
	}

	bool bNeedNewActiveCam = cameraToRemove->BIsSceneViewCam;
	bool bNeedNewDefaultCam = (_defaultCamera == nullptr) || (_defaultCamera == cameraToRemove);


	std::cout << "Erasing component: " << cameraToRemove->Name() << std::endl;
	_cameraComponents.erase(foundItterator);
	
	if (bNeedNewActiveCam)
	{
		if (bNeedNewDefaultCam) _defaultCamera = std::dynamic_pointer_cast<Camera>(_cameraComponents[0]); 
		_activeCamera = _defaultCamera;
		_activeCamera->BIsSceneViewCam = true;
	}

	return false;
}
