#pragma once
#include "Collider.h"
#include <EnTT/entt.hpp>
namespace Pixie
{
	struct CollisionComponent
	{
		CollisionComponent() = default;
		CollisionComponent(CollisionComponent&) = default;

		ColliderType Type{ ColliderType::END };
		bool BIsActive{ true };

		static const char* TypeNames[(unsigned long long)ColliderType::END];
		//testing a feature of entt I haven't used before
		static void on_construct(entt::registry& registry, const entt::entity entt);
		static void on_update(entt::registry& registry, const entt::entity entt);
		static void on_destroy(entt::registry& registry, const entt::entity entt);

		static void RemoveAllColliders(entt::registry& registry, const entt::entity entt);
	};
}

