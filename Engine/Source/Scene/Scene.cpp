#include "BsPrecompileHeader.h"
#include "Scene.h"
#include "GameObject.h"
#include "Scene/Components/Component.h"
#include "Scene/Components/MeshComponent.h"
#include "Graphics/Primitives/Cube.h"
#include "Components/Transform.h"
#include "Components/CameraController.h"
#include "Components/CameraComponent.h"
#include "EngineContext.h"
#include "Entity.h"

namespace Pixie
{
	Scene::Scene() 
	{
		m_CameraManager = CameraManager{ this, false };
	}

	void Scene::Initialize()
	{
		GameObject mainCam = CreateEmptyGameObject("Main Camera");
		CameraComponent& camera = mainCam.AddComponent<CameraComponent>();
		TransformComponent& transform = mainCam.GetComponent<TransformComponent>();
		transform.SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));
		transform.SetRotationEuler(glm::vec3(90.0f, 0.0f, 0.0f), AngleType::Degrees);

		mainCam.AddComponent<CameraController, entt::entity>(mainCam.GetEnttHandle());

		//DefaultCamera = mainCam;
		SetActiveCamera(mainCam);
	}

	void Scene::PopulateWithTestObjects()
	{
		GameObject cube1 = CreateCube();

		GameObject house = CreateEmptyGameObject("Viking House");
		TransformComponent& transform2 = house.GetComponent<TransformComponent>();
		transform2.SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
		transform2.SetScale(glm::vec3(20.0f));
		house.AddComponent<MeshComponent, const  std::string&>("../Assets/Meshes/Viking_House.obj");

		cube1.AddChild(house);
	}

	Scene::~Scene()
	{
		// unload scene objects that do not persist across scenes

	}

	void Scene::OnUpdate(float deltaTime)
	{
		m_CameraManager.OnEditorUpdate(deltaTime);

	}

	bool Scene::OnEvent(Event& event)
	{
		return m_CameraManager.OnEvent(event);
	}

	GameObject Scene::CreateEmptyGameObject(const std::string& name)
	{
		GameObject gameObject = { m_Registry.create(), this };
		gameObject.AddComponent<TransformComponent>();
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
		if (!m_Registry.valid(objectToRemove))
		{
			std::cout << "Tried to remove game object entt handle: " << int(objectToRemove) << ", from Scene, but could not find it" << std::endl;
			return;
		}

		m_Registry.destroy(objectToRemove);
	}

	void Scene::RemoveEntity(entt::entity entityToDelete)
	{
		if (!m_Registry.valid(entityToDelete))
		{
			std::cout << "Tried to remove game object entt handle: " << int(entityToDelete) << ", from Scene, but could not find it" << std::endl;
			return;
		}

		if (m_Registry.try_get<CameraComponent>(entityToDelete))
		{
			if (!TryRemoveCamera(entityToDelete))
			{
				return;
			}
		}

		//check if need to unparent
		HeirarchyComponent* family = m_Registry.try_get<HeirarchyComponent>(entityToDelete);
		if (family && family->Parent != entt::null)
		{
			GameObject parent{ family->Parent, this };
			parent.RemoveChild(entityToDelete);
		}

		m_Registry.destroy(entityToDelete);
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

	void Scene::ForwardAspectRatio(float width, float height)
	{
		m_CameraManager.OnViewportSizeEvent(width, height);
	}

	Camera* Scene::GetActiveCamera()
	{
		/*Camera* mainCamera = nullptr;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			CameraComponent& camera = view.get<CameraComponent>(entity);

			if (camera.IsPrimaryCamera)
			{
				mainCamera = &camera.Cam;
				break;
			}
		}

		return mainCamera;*/
		return m_CameraManager.GetActiveCamera();
	}

	Camera* Scene::GetActiveCamera(glm::mat4& viewMatrix)
	{
		//Camera* mainCamera = nullptr;
		/*
		auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (auto entity : group)
		{
			auto [camera, transform] = group.get<CameraComponent, TransformComponent>(entity);

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
		*/

		return m_CameraManager.GetActiveCamera(viewMatrix);
	}

	GameObject Scene::GetActiveCameraGameObject()
	{
		return m_CameraManager.GetActiveCameraObject();
	}

	void Scene::SetActiveCamera(GameObject& gameObject)
	{
		m_CameraManager.SetActiveCamera(gameObject);
		/*if (m_ActiveCamera == gameObject) return;
		
		if (m_ActiveCamera != entt::null)
		{
			CameraComponent& previousCamera = m_Registry.get<CameraComponent>(m_ActiveCamera);
			previousCamera.IsPrimaryCamera = false;
		}

		CameraComponent& nextCamera = gameObject.GetComponent<CameraComponent>();
		nextCamera.IsPrimaryCamera = true;
		m_ActiveCamera = gameObject;*/
	}

	void Scene::SetDefaultCamera(GameObject& gameObject)
	{
		/*if (m_DefaultCamera == gameObject) return;

		CameraComponent& nextCamera = gameObject.GetComponent<CameraComponent>();
		m_DefaultCamera = gameObject;*/
		m_CameraManager.SetDefaultCamera(gameObject);
	}

	bool Scene::TryRemoveCamera(entt::entity entityHandle)
	{
	/*	auto view = m_Registry.view<CameraComponent>();
		if (view.size() < 2)
		{
			std::cout << "Tried to remove Camera Object with handle: " << int(entityHandle) << ", from Scene, but there are not any cameras to replace it with" << std::endl;
			return false;
		}*/

		/*if (entityHandle == m_DefaultCamera)
		{
			for (auto entity : view)
			{
				if (entity != entityHandle)
				{
					m_DefaultCamera = entity;
					break;
				}
			}
		}

		if (entityHandle == m_ActiveCamera)
		{
			m_ActiveCamera = m_DefaultCamera;
			m_Registry.get<CameraComponent>(m_ActiveCamera).IsPrimaryCamera = true;
		}*/

		if (!m_CameraManager.IsCameraRemovable(entityHandle)) return false;

		m_Registry.remove<CameraComponent>(entityHandle);
		m_CameraManager.OnRemoveCamera(entityHandle);

		return true;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
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


	template<typename T>
	inline void Scene::OnComponentAdded(Entity entity, T& component)
	{

	}

	template<>
	inline void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		glm::vec2 viewport = EngineContext::GetEngine()->GetViewportSize();
		component.Cam.SetAspectRatio(viewport.x / viewport.y);
		
	}
}