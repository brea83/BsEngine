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

	void SetParent(entt::entity newParent, bool bSentFromAddChild = false);
	GameObject GetParent();

	void UnParent(entt::entity grandParent = entt::null, bool bKeepWorldPosition = true);

	void AddChild(entt::entity child, bool bSentFromSetParent = false);
	void RemoveChild(entt::entity child);
	std::vector< GameObject>& GetChildren();

	virtual void OnUpdate(){ }

protected:
	//GameObject* m_Parent{ nullptr };
	//std::vector<GameObject*> m_Children;

};
