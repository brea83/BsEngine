#pragma once
#include "BsPrecompileHeader.h"
#include <glm/glm.hpp>
#include "Component.h"

class Transform;

class GameObject
{
public:
	GameObject(std::string name = "Default GameObject", glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
	virtual ~GameObject() { }
	std::string Name;

	virtual std::shared_ptr<Transform> GetTransform() { return _transform; }

	template <typename Type, typename... Args>
	Type& AddComponent(Args&&... args)
	{
		//TODO: add check for adding duplicate component
		Component* component = dynamic_cast<Component>(new Type(args));
		if (component != nullptr)
		{
			_components[component->Name] = component;
		}
	}

protected:
	std::shared_ptr<Transform> _transform{ nullptr };

	std::unordered_map<std::string, Component*> _components;

	virtual void Init();
};

