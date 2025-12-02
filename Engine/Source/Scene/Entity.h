#pragma once
#include "Scene.h"
#include <EnTT/entt.hpp>

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, Scene* scene);
	Entity(const Entity& other) = default;


	template <typename Type>
	bool HasCompoenent()
	{
		return m_Scene->GetRegistry().any_of<Type>(m_EntityHandle);
	}

	// args are used to construct the component of <Type>
	template <typename Type, typename... Args>
	Type&  AddComponent(Args&&... args)
	{
		if (HasCompoenent<Type>()) std::cout << "WARNING: CURRENTLY ADDING COMPONENT OF DUPLICATE TYPE WILL OVERWRITE THE OLD COMPOENENT" << std::endl;
		return m_Scene->GetRegistry().emplace_or_replace<Type>(m_EntityHandle, std::forward<Args>(args)...);
	}

	template <typename Type>
	Type& GetComponent()
	{
		if (!HasCompoenent<Type>()) std::cout << "WARNING: COULD NOT FIND COMPONENT" << std::endl;
		return m_Scene->GetRegistry().get<Type>(m_EntityHandle);
	}

	//std::unordered_map<size_t, std::shared_ptr<Component>>& GetAllComponents() { return _components; }

	template <typename Type>
	void RemoveComponent()
	{
		if (!HasCompoenent<Type>()) return;
		m_Scene->GetRegistry().remove<Type>(m_EntityHandle);
	}

	operator bool() const { return m_Scene->GetRegistry().valid(m_EntityHandle); }
protected:
	Scene* m_Scene{ nullptr };
	entt::entity m_EntityHandle{ entt::null };
};

