#include "BsPrecompileHeader.h"
#include "Scene.h"
#include "GameObject.h"
#include "Scene/Components/Component.h"
#include "Scene/Components/MeshComponent.h"
#include "Graphics/Primitives/Cube.h"
#include "Graphics/Primitives/Transform.h"
#include "Graphics/CameraController.h"
#include "Components/CameraComponent.h"
#include "EngineContext.h"
#include "Entity.h"

Scene::Scene()
{ 

	/*GameObject* cameraObject = CreateEmptyGameObject();
	cameraObject->Name = "Main Camera";
	std::shared_ptr<Transform> transform = cameraObject->GetTransform();
	transform->SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	transform->SetRotationEuler(glm::vec3(90.0f, 0.0f, 0.0f), AngleType::Degrees);
	
	m_ActiveCamera = cameraObject->AddComponent<CameraComponent>();

	m_ActiveCamera->BIsSceneViewCam = true;*/
}

void Scene::Initialize()
{
	Entity mainCam = CreateEntity("Main Camera");
	mainCam.AddComponent<HeirarchyComponent>();
	CameraComponent& camera = mainCam.AddComponent<CameraComponent>();
	Transform& transform = mainCam.GetComponent<Transform>();
	transform.SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	transform.SetRotationEuler(glm::vec3(90.0f, 0.0f, 0.0f), AngleType::Degrees);

	mainCam.AddComponent<CameraController, entt::entity>(mainCam.GetEnttHandle());

	m_DefaultCamera = mainCam.GetEnttHandle();//std::make_shared<EditorCamera>(camera.Cam);
	m_ActiveCamera = m_DefaultCamera;
}

Scene::~Scene()
{
	// unload scene objects that do not persist across scenes

}

void Scene::OnUpdate(float deltaTime)
{
	
}

GameObject Scene::CreateEmptyGameObject(const std::string& name)
{
	GameObject gameObject = { m_Registry.create(), this };
	gameObject.AddComponent<Transform>();
	gameObject.AddComponent<HeirarchyComponent>();
	NameComponent& nameComponent = gameObject.AddComponent<NameComponent>();
	nameComponent.Name = name.empty() ? "Empty Object" : name;
	return gameObject;
}
//
//void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
//{
//	m_GameObjects.push_back(gameObject);
//}

void Scene::RemoveGameObject(GameObject objectToRemove)
{
	if (m_Registry.valid(objectToRemove))
	{
		std::cout << "Tried to remove game object entt handle: " << int(objectToRemove) << ", from Scene, but could not find it" << std::endl;
		return;
	}

	m_Registry.destroy(objectToRemove);
}

void Scene::RemoveEntity(entt::entity entityHandle)
{
	if (m_Registry.valid(entityHandle))
	{
		std::cout << "Tried to remove game object entt handle: " << int(entityHandle) << ", from Scene, but could not find it" << std::endl;
		return;
	}

}

GameObject Scene::GetGameObjectByEntityHandle(entt::entity entityHandle)
{
	if (m_Registry.valid(entityHandle))
	{
		return GameObject(entityHandle, this);
	}
	return GameObject();
}

GameObject Scene::FindGameObjectByName(const std::string& name)
{
	auto view = m_Registry.view<NameComponent>();
	for (auto entity : view)
	{
		const NameComponent& component = view.get<NameComponent>(entity);
		if (component.Name == name)
		{
			return GameObject(entity, this);
		}
	}

	return GameObject();
}

GameObject Scene::DuplicateGameObject(GameObject object)
{
	return GameObject();
}

Camera* Scene::GetActiveCamera()
{
	Camera* mainCamera = nullptr;

	auto group = m_Registry.group<CameraComponent>(entt::get<Transform>);
	for (auto entity : group)
	{
		auto [camera, transform] = group.get<CameraComponent, Transform>(entity);

		if (camera.IsPrimaryCamera)
		{
			mainCamera = &camera.Cam;
			break;
		}
	}

	return mainCamera;
}

Camera* Scene::GetActiveCamera(glm::mat4& viewMatrix)
{
	Camera* mainCamera = nullptr;

	auto group = m_Registry.group<CameraComponent>(entt::get<Transform>);
	for (auto entity : group)
	{
		auto [camera, transform] = group.get<CameraComponent, Transform>(entity);

		if (camera.IsPrimaryCamera)
		{
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
			
			mainCamera = &camera.Cam;
			break;
		}
	}

	return mainCamera;
}

GameObject Scene::GetActiveCameraGameObject()
{
	return GameObject(m_ActiveCamera, this);
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity = { m_Registry.create(), this };
	entity.AddComponent<Transform>();
	NameComponent& nameComponent = entity.AddComponent<NameComponent>();
	nameComponent.Name = name.empty() ? "Empty Object" : name;
	return entity;
}

GameObject Scene::CreateCube()
{
	//m_MeshComponents.emplace_back(new Cube());
	GameObject object = CreateEmptyGameObject("Cube");
	object.AddComponent<MeshComponent, PrimitiveMeshType>(PrimitiveMeshType::Cube);
	//m_GameObjects.push_back(std::make_shared<GameObject>(object));
	return object;
}
//
//void Scene::RemoveRenderable(std::shared_ptr<MeshComponent> modelToRemove)
//{
//	auto foundItterator = std::find(m_MeshComponents.begin(), m_MeshComponents.end(), modelToRemove);
//
//	if (foundItterator != m_MeshComponents.end())
//	{
//		std::cout << "FOUND OBJECT: " << modelToRemove->Name() << std::endl;
//		m_MeshComponents.erase(foundItterator);
//	}
//	else
//	{
//		std::cout << modelToRemove->Name() << ", NOT FOUND" << std::endl;
//	}
//}

//bool Scene::TryRemoveCamera(std::shared_ptr<CameraComponent> cameraToRemove)
//{
//	if (m_CameraComponents.size() < 2)
//	{
//		std::cout << "Error: tried to delete last camera component, but scene requires minimum of one camera" << std::endl;
//		return false;
//	}
//
//	auto foundItterator = std::find(m_CameraComponents.begin(), m_CameraComponents.end(), cameraToRemove);
//
//	if (foundItterator == m_CameraComponents.end())
//	{
//		//std::cout << cameraToRemove->Name() << ", NOT FOUND, can't remove it" << std::endl;
//		return false;
//	}
//
//	//bool bNeedNewActiveCam = cameraToRemove->BIsSceneViewCam;
//	bool bNeedNewDefaultCam = (m_DefaultCamera == nullptr) || (m_DefaultCamera == cameraToRemove);
//
//
//	//std::cout << "Erasing component: " << cameraToRemove->Name() << std::endl;
//	m_CameraComponents.erase(foundItterator);
//	
//	/*if (bNeedNewActiveCam)
//	{
//		if (bNeedNewDefaultCam) m_DefaultCamera = std::dynamic_pointer_cast<Camera>(m_CameraComponents[0]); 
//		m_ActiveCamera = m_DefaultCamera;
//		m_ActiveCamera->BIsSceneViewCam = true;
//	}*/
//
//	return false;
//}
