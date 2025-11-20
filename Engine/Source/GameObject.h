#pragma once
#include "BsPrecompileHeader.h"
#include <glm/glm.hpp>
#include "Component.h"

class Transform;


class GameObject
{
public:
	GameObject(std::string name = "Default GameObject", glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
	virtual ~GameObject();
	
	std::string Name;

	virtual std::shared_ptr<Transform> GetTransform() { return _transform; }

	template <typename Type>
	bool HasCompoenent()
	{
		return _components.find(typeid(Type).hash_code()) != _components.end();
	}

	// args are used to construct the component of <Type>
	template <typename Type, typename... Args>
	std::shared_ptr<Type>  AddComponent(Args&&... args)
	{
		if (HasCompoenent<Type>())
		{
			std::cout << "WARNING: CURRENTLY ADDING COMPONENT OF DUPLICATE TYPE WILL OVERWRITE THE OLD COMPOENENT" << std::endl;
		}
		std::shared_ptr<Type>  component = std::make_shared<Type>(this, std::forward<Args>(args)...);
		//Component baseComponent = dynamic_cast<Component>(new Type(args));

		_components[typeid(Type).hash_code()] = component;
		OnComponentAdded(component);
		return component;

	}

	template <typename Type>
	std::shared_ptr<Type> GetComponent()
	{
		//get hashcode key to look for
		if (HasCompoenent<Type>())
		{
			return _components[typeid(Type).hash_code()];
		}
		
	}

	template <typename Type>
	void RemoveComponent()
	{
		if (HasCompoenent<Type>())
		{
			_components.erase(typeid(Type).hash_code());
		}
	}

	void SetParent(std::shared_ptr<GameObject> newParent);
	std::shared_ptr<GameObject> GetParent() { return _parent; }

	void AddChild(std::shared_ptr<GameObject> child);
	std::vector< std::shared_ptr<GameObject>>& GetChildren() { return _children; }

	virtual void OnUpdate(){ }

	bool operator ==(const GameObject& other)
	{
		return Name == other.Name
			//TODO: ADD ID COMPARISSON
			&& _parent == other._parent;
	}

protected:
	std::shared_ptr<Transform> _transform{ nullptr };

	//for now components are hashed by their typeid hashcode, meaning no duplicates of class type. this will need to be modified
	std::unordered_map<size_t, std::shared_ptr<Component>> _components;

	std::shared_ptr<GameObject> _parent{ nullptr };
	std::vector<std::shared_ptr<GameObject>> _children;

	virtual void Init();

	virtual void OnComponentAdded(std::shared_ptr<Component> component);
};

