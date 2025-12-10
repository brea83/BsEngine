#include "BsPrecompileHeader.h"
#include "Scene.h"
#include "GameObject.h"
#include "Scene/Components/Component.h"
#include "Scene/Components/MeshComponent.h"
#include "Graphics/Primitives/Cube.h"
#include "Graphics/Primitives/Transform.h"
#include "Editor/EditorCamera.h"
#include "Components/CameraComponent.h"
#include "EngineContext.h"
#include "Entity.h"

namespace Pixie
{
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
		NewCameraComponent& camera = mainCam.AddComponent<NewCameraComponent>();
		m_DefaultCamera = std::make_shared<EditorCamera>(camera.Cam);
		m_ActiveCamera = m_DefaultCamera;
	}

	Scene::~Scene()
	{
		// unload scene objects that do not persist across scenes
		if (!m_GameObjects.empty())
		{
			for (GameObject* gameObject : m_GameObjects)
			{
				delete gameObject;
			}
		}
	}

	GameObject* Scene::CreateEmptyGameObject()
	{
		GameObject* newObject = new GameObject();
		m_GameObjects.push_back(newObject);
		return newObject;
	}

	void Scene::RemoveGameObject(GameObject* objectToRemove)
	{
		if (objectToRemove == nullptr) return;

		auto itterator = std::find(m_GameObjects.begin(), m_GameObjects.end(), objectToRemove);
		if (itterator != m_GameObjects.end())
		{

			m_GameObjects.erase(itterator);
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

		return m_GameObjects[index];

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<Transform>();
		TagComponent& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Empty Object" : name;
		return entity;
	}

	void Scene::CreateCube()
	{
		//m_MeshComponents.emplace_back(new Cube());
		GameObject* object = new GameObject("Cube");
		AddGameObject(object);
		object->AddComponent<MeshComponent, PrimitiveMeshType>(PrimitiveMeshType::Cube);
	}

	void Scene::RemoveRenderable(std::shared_ptr<MeshComponent> modelToRemove)
	{
		auto foundItterator = std::find(m_MeshComponents.begin(), m_MeshComponents.end(), modelToRemove);

		if (foundItterator != m_MeshComponents.end())
		{
			std::cout << "FOUND OBJECT: " << modelToRemove->Name() << std::endl;
			m_MeshComponents.erase(foundItterator);
		}
		else
		{
			std::cout << modelToRemove->Name() << ", NOT FOUND" << std::endl;
		}
	}

	bool Scene::TryRemoveCamera(std::shared_ptr<CameraComponent> cameraToRemove)
	{
		if (m_CameraComponents.size() < 2)
		{
			std::cout << "Error: tried to delete last camera component, but scene requires minimum of one camera" << std::endl;
			return false;
		}

		auto foundItterator = std::find(m_CameraComponents.begin(), m_CameraComponents.end(), cameraToRemove);

		if (foundItterator == m_CameraComponents.end())
		{
			std::cout << cameraToRemove->Name() << ", NOT FOUND, can't remove it" << std::endl;
			return false;
		}

		bool bNeedNewActiveCam = cameraToRemove->BIsSceneViewCam;
		bool bNeedNewDefaultCam = (m_DefaultCamera == nullptr) || (m_DefaultCamera == cameraToRemove);


		std::cout << "Erasing component: " << cameraToRemove->Name() << std::endl;
		m_CameraComponents.erase(foundItterator);

		if (bNeedNewActiveCam)
		{
			if (bNeedNewDefaultCam) m_DefaultCamera = std::dynamic_pointer_cast<Camera>(m_CameraComponents[0]);
			m_ActiveCamera = m_DefaultCamera;
			m_ActiveCamera->BIsSceneViewCam = true;
		}

		return false;
	}
}