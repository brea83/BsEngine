#include "BsPrecompileHeader.h"
#include "Scene.h"
#include "GameObject.h"
#include "Graphics/Primitives/Cube.h"

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
		//GameObject mainCam = CreateEmptyGameObject("Main Camera");
		//CameraComponent& camera = mainCam.AddComponent<CameraComponent>();
		//TransformComponent& transform = mainCam.GetComponent<TransformComponent>();
		//transform.SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));
		//transform.SetRotationEuler(glm::vec3(90.0f, 0.0f, 0.0f), AngleType::Degrees);

		//mainCam.AddComponent<CameraController, entt::entity>(mainCam.GetEnttHandle());

		////DefaultCamera = mainCam;
		//SetActiveCamera(mainCam);
		// TODO: make catch so that editor camera isn't created and used in non editor modes
		m_CameraManager.InitEditor();
		
		auto cameras = m_Registry.view<CameraComponent>();
		if (cameras)
		{
			GameObject firstCamera(cameras.front(), this);
			SetActiveCamera(firstCamera);
		}
	}

	void Scene::PopulateWithTestObjects()
	{
		GameObject mainLight = CreateEmptyGameObject("Main Light");
		LightComponent& light = mainLight.AddComponent<LightComponent>();
		light.Type = LightType::Directional;

		GameObject cube1 = CreateCube();

		GameObject house = CreateEmptyGameObject("Viking House");
		TransformComponent& transform2 = house.GetComponent<TransformComponent>();
		transform2.SetPosition(glm::vec3(2.5f, -0.5f, 0.0f));
		transform2.SetScale(glm::vec3(20.0f));
		house.AddComponent<MeshComponent, const  std::string&, const  std::string&>("../Assets/Meshes/Viking_House.obj", "../Assets/Textures/Viking_House.png");

		GameObject sphere = CreateEmptyGameObject("Sphere");
		TransformComponent& transform3 = sphere.GetComponent<TransformComponent>();
		transform3.SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
		sphere.AddComponent<MeshComponent, const std::string&>("../Assets/Meshes/Sphere.obj");

		cube1.AddChild(sphere);

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
		gameObject.AddComponent<IDComponent>();
		gameObject.AddComponent<TransformComponent>();
		gameObject.AddComponent<HeirarchyComponent>();
		NameComponent& nameComponent = gameObject.AddComponent<NameComponent>();
		nameComponent.Name = name.empty() ? "Empty Object" : name;
		return gameObject;
	}
	
	GameObject Scene::CreateGameObjectWithGUID(GUID guid, const std::string& name)
	{
		GameObject gameObject = { m_Registry.create(), this };
		gameObject.AddComponent<IDComponent>(guid);
		gameObject.AddComponent<TransformComponent>();
		gameObject.AddComponent<HeirarchyComponent>();
		NameComponent& nameComponent = gameObject.AddComponent<NameComponent>();
		nameComponent.Name = name.empty() ? "Empty Object" : name;
		return gameObject;
	}

	void Scene::RemoveGameObject(GameObject objectToRemove)
	{
		if (!m_Registry.valid(objectToRemove))
		{
			Logger::Log(LOG_WARNING, "Tried to remove game object with entt handle: {}, from Scene, but could not find it", int(objectToRemove));
			return;
		}

		if (objectToRemove.TryGetComponent<CameraComponent>())
		{
			if (!TryRemoveCamera(objectToRemove))
			{
				return;
			}
		}

		//check if need to unparent
		GameObject parent = objectToRemove.GetParent();
		//HeirarchyComponent* family = m_Registry.try_get<HeirarchyComponent>(objectToRemove);
		if (parent)
		{
			parent.RemoveChild(objectToRemove);
		}

		std::vector<GameObject> children = objectToRemove.GetChildren();
		for (GameObject child : children)
		{
			if (parent)
				child.SetParent(parent);
			else
				child.SetParentNone();
		}

		m_Registry.destroy(objectToRemove);
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

	GameObject Scene::FindGameObjectByGUID(GUID guid)
	{
		auto view = m_Registry.view<IDComponent>();
		for (auto entity : view)
		{
			const IDComponent& component = view.get<IDComponent>(entity);
			if (component.ID == guid)
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
		return m_CameraManager.GetActiveCamera();
	}

	Camera* Scene::GetActiveCamera(glm::mat4& viewMatrix)
	{
		return m_CameraManager.GetActiveCamera(viewMatrix);
	}

	GameObject Scene::GetActiveCameraGameObject()
	{
		return m_CameraManager.GetActiveCameraObject();
	}

	void Scene::SetActiveCamera(GameObject& gameObject)
	{
		m_CameraManager.SetActiveCamera(gameObject);
		
	}

	void Scene::SetDefaultCamera(GameObject& gameObject)
	{
		m_CameraManager.SetDefaultCamera(gameObject);
	}

	bool Scene::TryRemoveCamera(entt::entity entityHandle)
	{
		if (!m_CameraManager.IsCameraRemovable(entityHandle)) return false;

		m_Registry.remove<CameraComponent>(entityHandle);
		m_CameraManager.OnRemoveCamera(entityHandle);

		return true;
	}

	GameObject Scene::GetMainLight()
	{
		auto view = m_Registry.view<LightComponent>();
		if (!view) return GameObject();

		std::vector<entt::entity> directionalLights;
		for (auto entity : view)
		{
			LightComponent& light = view.get<LightComponent>(entity);
			if (light.Type == Directional) directionalLights.push_back(entity);
		}

		if (directionalLights.size() < 1) return GameObject();
		
		if(directionalLights.size() > 1) Logger::Log(LOG_WARNING, "Warning: more than one directional light in scene, but Scene::GetMainLight() only returns the first one found");

		return GameObject(directionalLights.front(), this);
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		//entity.AddComponent<TransformComponent>();
		NameComponent& nameComponent = entity.AddComponent<NameComponent>();
		nameComponent.Name = name.empty() ? "Empty Entity" : name;
		return entity;
	}

	GameObject Scene::CreateCube()
	{
		//m_MeshComponents.emplace_back(new Cube());
		GameObject object = CreateEmptyGameObject("Cube");
		//object.AddComponent<MeshComponent, PrimitiveMeshType>(PrimitiveMeshType::Cube);
		object.AddComponent<MeshComponent, const std::string&>("../Assets/Meshes/Cube.obj");
		return object;
	}


	template<typename T>
	 void Scene::OnComponentAdded(Entity& entity, T& component)
	{

	}

	 template<>
	 void Scene::OnComponentAdded<IDComponent>(Entity& entity, IDComponent& component)
	 {}

	template<>
	 void Scene::OnComponentAdded<TagComponent>(Entity& entity, TagComponent& component)
	{}

	template<>
	 void Scene::OnComponentAdded<NameComponent>(Entity& entity, NameComponent & component)
	{}

	template<>
	 void Scene::OnComponentAdded<HeirarchyComponent>(Entity& entity, HeirarchyComponent & component)
	{}

	template<>
	 void Scene::OnComponentAdded<TransformComponent>(Entity& entity, TransformComponent & component)
	{}

	template<>
	 void Scene::OnComponentAdded<MeshComponent>(Entity& entity, MeshComponent & component)
	{}

	template<>
	 void Scene::OnComponentAdded<LightComponent>(Entity& entity, LightComponent & component)
	{}

	template<>
	 void Scene::OnComponentAdded<CameraComponent>(Entity& entity, CameraComponent& component)
	{
		/*glm::vec2 viewport = EngineContext::GetEngine()->GetViewportSize();
		component.Cam.SetAspectRatio(viewport.x / viewport.y);*/

		m_CameraManager.OnCameraAdded(entity.GetEnttHandle(), component);
	}

	template<>
	 void Scene::OnComponentAdded<CameraController>(Entity& entity, CameraController& component)
	{}
	
}