#pragma once
#include "Scene.h"
#include "Components/Component.h"
#include <EnTT/entt.hpp>

class Entity
{
public:
	Entity(){}

	template <typename Type>
	bool HasCompoenent();

	// args are used to construct the component of <Type>
	template <typename Type, typename... Args>
	std::shared_ptr<Type>  AddComponent(Args&&... args);

	template <typename Type>
	std::shared_ptr<Type> GetComponent();

	//std::unordered_map<size_t, std::shared_ptr<Component>>& GetAllComponents() { return _components; }

	template <typename Type>
	void RemoveComponent();

protected:
	entt::entity m_EntityHandle;
};

