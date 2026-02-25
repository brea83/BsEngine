#pragma once
//#include "Core.h"
#include "BsPrecompileHeader.h"
#include "Scene/Entity.h"
#include "Scene/Components/Component.h"

#include <functional>
//#include "Resources/FileStream.h"

namespace Pixie
{
	class TransformComponent;

	class GameObject : public Entity
	{
	public:
		GameObject() : Entity(){}
		GameObject(entt::entity entity, std::shared_ptr<Scene> scene);
		virtual ~GameObject();
		
		TransformComponent& GetTransform();

		GUID GetGUID() const { return GetComponent<IDComponent>().ID; }
		

		//std::shared_ptr<Scene> GetScene() { return m_Scene; }
		

		void SetParentNone();
		void SetParent(GameObject& newParent, bool bSentFromAddChild = false);
		GameObject GetParent();

		//void UnParent(GameObject grandParent, bool bKeepWorldPosition = true);

		void AddChild(GameObject& child, bool bSentFromSetParent = false);
		void RemoveChild(GameObject& child);
		std::vector< GameObject> GetChildren();

		template <typename T>
		std::vector<GameObject> FindObjectsByComponent(const std::string tag = "");

		template <typename T>
		T* GetComponentInParent();

		template<typename T>
		std::vector<GameObject> GetChildrenWithComponent(bool andInSelf = true);

		virtual void OnCreate(){ }
		virtual void OnBeginPlay();
		virtual void OnUpdate(float deltaTime);
		virtual void OnDestroy(){ }

		virtual void TryDestroy();

		// Serialized as:
		// entt ID (to do replace with uid)
		// list of serializable component IDs
		// loop through list of component ids to write component data
		// Note need to update with new component types as they are added
		static void Serialize(StreamWriter* fileWriter, const GameObject& object);
		// Serialized as:
		// entt ID (to do replace with uid)
		// list of serializable component IDs
		// loop through list of component ids to create or replace components
		// Note need to update with new component types as they are added
		static bool Deserialize(StreamReader* fileReader, GameObject& object);
	protected:
		//entt::entity m_SerializedID{ entt::null };
		
		//very basic movement called on update, and does not include turning. override for game or entity specific behavior.
		virtual bool Move(float deltaTime);
		// basic handling of object movment from the following components MovementComponent, FollowComponent, and Orbit Component
		// Returns the next desired position as an offset from current position intended to be added.
		virtual InterpolatedTransform HandleMovementComponents(float deltaTime);
	private:
		friend class Scene;
	};


	template <>
	inline void Entity::RemoveComponent<NativeScriptComponent>()
	{
		if (!HasCompoenent<NativeScriptComponent>()) return;

		NativeScriptComponent& scriptComponent = GetComponent<NativeScriptComponent>();
		GameObject object { m_EntityHandle, m_Scene };
		for (auto pair : scriptComponent.AttachedScripts)
		{
			if(pair.second.RemoveComponent)
				pair.second.RemoveComponent(object);
		}

		m_Scene->GetRegistry().remove<NativeScriptComponent>(m_EntityHandle);

	}

	template <typename T>
	inline std::vector<GameObject> GameObject::FindObjectsByComponent(const std::string tag)
	{
		entt::registry& registry = m_Scene->GetRegistry();

		std::vector<GameObject> results;

		if (tag.empty())
		{
			// search only by component type
			auto view = registry.view<T>();
			for (auto entity : view)
			{
				results.emplace_back(entity, m_Scene);
			}
		}
		else
		{
			for (auto&& [entity, component, tagComponent] : registry.view<T, TagComponent>().each())
			{
				if (tagComponent.Tag == tag)
				{
					results.emplace_back(entity, m_Scene);
				}
			}
		}

		return results;
	}

	template<typename T>
	inline T* Pixie::GameObject::GetComponentInParent()
	{
		GameObject parent = GetParent();
		if (!parent)
			return nullptr;

		return parent.TryGetComponent<T>();
	}

	template<typename T>
	inline std::vector<GameObject> Pixie::GameObject::GetChildrenWithComponent(bool andInSelf)
	{
		std::vector<GameObject> results;

		if (andInSelf && this->HasCompoenent<T>())
		{
			results.push_back(*this);
		}

		std::vector<GameObject> children = GetChildren();

		for (GameObject& child : children)
		{
			if (child.HasCompoenent<T>())
				results.push_back(child);
		}
		return results;
	}

}
