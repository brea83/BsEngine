#pragma once
#include "Core.h"
#include "BsPrecompileHeader.h"
#include <glm/glm.hpp>
#include "Scene/Entity.h"

namespace Pixie
{
	class TransformComponent;

	class GameObject : public Entity
	{
	public:
		GameObject() : Entity(){}
		GameObject(entt::entity entity, Scene* scene);
		virtual ~GameObject();
		
		virtual void Init();
		TransformComponent& GetTransform();

		void SetParent(entt::entity newParent, bool bSentFromAddChild = false);
		GameObject GetParent();

		void UnParent(entt::entity grandParent = entt::null, bool bKeepWorldPosition = true);

		void AddChild(entt::entity child, bool bSentFromSetParent = false);
		void RemoveChild(entt::entity child);
		std::vector< GameObject>& GetChildren();

		virtual void OnUpdate(float deltaTime);

	protected:

	};
}
