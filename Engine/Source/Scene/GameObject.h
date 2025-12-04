#pragma once
#include "BsPrecompileHeader.h"
#include <glm/glm.hpp>
#include "Scene/Entity.h"
//class Transform;


class GameObject : public Entity
{
public:
	GameObject() : Entity(){}
	GameObject(entt::entity entity, Scene* scene);
	virtual ~GameObject();
	
	virtual void Init();
	//virtual Transform& GetTransform();

	void SetParent(GameObject* newParent, bool bSentFromAddChild = false);
	GameObject* GetParent() { return m_Parent; }

	void UnParent(GameObject* grandParent = nullptr, bool bKeepWorldPosition = true);

	void AddChild(GameObject* child, bool bSentFromSetParent = false);
	void RemoveChild(GameObject* child);
	std::vector< GameObject*>& GetChildren() { return m_Children; }

	virtual void OnUpdate(){ }

protected:
	GameObject* m_Parent{ nullptr };
	std::vector<GameObject*> m_Children;

};
