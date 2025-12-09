#pragma once
#include "Scene.h"
#include <EnTT/entt.hpp>
#include "Scene/Components/CameraComponent.h"

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, Scene* scene);
	Entity(const Entity& other) = default;

	entt::entity GetEnttHandle() const { return m_EntityHandle; }

	// Component function templates defined lower in the file to make 
	// reading what functions are available less cluttered
	template <typename Type>
	bool HasCompoenent();

	// args are used to construct the component of <Type>
	template <typename Type, typename... Args>
	Type& AddComponent(Args&&... args);

	template<typename Type, typename... Args>
	Type& AddOrReplaceComponent(Args&&... args);

	template <typename Type>
	Type& GetComponent();
	template <typename Type>
	Type* TryGetComponent();

	template <typename Type>
	void RemoveComponent();
	template<>
	void RemoveComponent<CameraComponent>();

	// operator overrides

	operator bool() const { return m_Scene->GetRegistry().valid(m_EntityHandle); }
	operator entt::entity() const { return m_EntityHandle; }
	operator int() const { return static_cast<int>(m_EntityHandle); }
	operator uint32_t() const { return (uint32_t)m_EntityHandle; }

	bool operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

protected:
	Scene* m_Scene{ nullptr };
	entt::entity m_EntityHandle{ entt::null };

private:
	friend class SceneHierarchyPanel;

};



template <typename Type>
inline bool Entity::HasCompoenent()
{
	return m_Scene->GetRegistry().any_of<Type>(m_EntityHandle);
}

// args are used to construct the component of <Type>
template <typename Type, typename... Args>
inline Type& Entity::AddComponent(Args&&... args)
{
	if (HasCompoenent<Type>()) std::cout << "WARNING: CURRENTLY ADDING COMPONENT OF DUPLICATE TYPE WILL OVERWRITE THE OLD COMPOENENT" << std::endl;
	Type& component = m_Scene->GetRegistry().emplace_or_replace<Type>(m_EntityHandle, std::forward<Args>(args)...);
	m_Scene->OnComponentAdded<Type>(*this, component);
	return component;
}

template<typename Type, typename... Args>
inline Type& Entity::AddOrReplaceComponent(Args&&... args)
{
	Type& component = m_Scene->GetRegistry().emplace_or_replace<Type>(m_EntityHandle, std::forward<Args>(args)...);
	m_Scene->OnComponentAdded<Type>(*this, component);
	return component;
}

template <typename Type>
inline Type& Entity::GetComponent()
{
	if (!HasCompoenent<Type>()) std::cout << "WARNING: COULD NOT FIND COMPONENT" << std::endl;
	return m_Scene->GetRegistry().get<Type>(m_EntityHandle);
}

template <typename Type>
inline Type* Entity::TryGetComponent()
{
	return m_Scene->GetRegistry().try_get<Type>(m_EntityHandle);
}


template <typename Type>
inline void Entity::RemoveComponent()
{
	if (!HasCompoenent<Type>()) return;

	m_Scene->GetRegistry().remove<Type>(m_EntityHandle);
}

template <>
inline void Entity::RemoveComponent<CameraComponent>()
{
	if (!HasCompoenent<CameraComponent>()) return;

	// error msging if unable to remove cam is handled in TryRemoveCamera()
	m_Scene->TryRemoveCamera(m_EntityHandle);

}