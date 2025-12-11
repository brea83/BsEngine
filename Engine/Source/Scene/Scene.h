#pragma once
#include "Core.h"
#include <glm/glm.hpp>
#include <EnTT/entt.hpp>
#include "Editor/EditorCamera.h"

namespace Pixie
{
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
		//void AddGameObject(std::shared_ptr<GameObject> gameObject);
		void RemoveGameObject(GameObject objectToRemove);
		void RemoveEntity(entt::entity entityHandle);
		//int NumGameObjects() { return m_GameObjects.size(); }
		
		GameObject GetGameObjectByEntityHandle(entt::entity entityHandle);
		GameObject FindGameObjectByName(const std::string& name);

		GameObject DuplicateGameObject(GameObject object);
		
		void Initialize();
		void PopulateWithTestObjects();

		//TODO figure out if an interface might solve sending updates to all updateable components?
		void OnUpdate(float deltaTime);

		entt::registry& GetRegistry() { return m_Registry; }
		Entity CreateEntity(const std::string& name = "");

		//void AddRenderable(std::shared_ptr<MeshComponent> newRenderable) { m_MeshComponents.push_back(newRenderable); }
		GameObject CreateCube();
		//void RemoveRenderable(std::shared_ptr<MeshComponent> modelToRemove);
		//std::vector<std::shared_ptr<MeshComponent>>& GetRenderables();

		//void AddCamera(std::shared_ptr<CameraComponent> camera) { m_CameraComponents.push_back(camera); }
		//bool TryRemoveCamera(std::shared_ptr<CameraComponent> cameraToRemove);
		Camera* GetActiveCamera();
		Camera* GetActiveCamera(glm::mat4& viewMatrix);
		GameObject GetActiveCameraGameObject();
		void SetActiveCamera(GameObject& gameObject);
		void SetDefaultCamera(GameObject& gameObject);
		//Transform& GetActiveCameraTransform();

		bool TryRemoveCamera(entt::entity entityHandle);

	private:
		entt::registry m_Registry;
		entt::entity m_ActiveCamera{ entt::null };
		entt::entity m_DefaultCamera{ entt::null };
		//EditorCamera m_EditorCamera;
		bool m_IsInitiated{ false };

		//std::vector<std::shared_ptr<CameraComponent>> m_CameraComponents;
		/*std::vector<std::shared_ptr<MeshComponent>> m_MeshComponents;
		std::vector<std::shared_ptr<GameObject>> m_GameObjects;*/

		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		template<>
		void OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component);

		friend class SceneHierarchyPanel;
		friend class DetailsViewPanel;
		friend class ImGuiLayer;
		friend class Entity;
	};
}
