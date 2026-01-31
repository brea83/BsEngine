#pragma once
#include "Collider.h"
#include "Resources/FileStream.h"
#include <EnTT/entt.hpp>
namespace Pixie
{


	struct CollisionComponent
	{
		CollisionComponent() = default;
		CollisionComponent(const CollisionComponent&) = default;

		ColliderType Type{ ColliderType::END };
		bool BIsActive{ true };

		static const char* TypeNames[(unsigned long long)ColliderType::END];

		//testing a feature of entt I haven't used before
		static void on_construct(entt::registry& registry, const entt::entity entt);
		static void on_update(entt::registry& registry, const entt::entity entt);
		static void on_destroy(entt::registry& registry, const entt::entity entt);

		static void RemoveAllColliders(entt::registry& registry, const entt::entity entt);

		static void Serialize(StreamWriter* stream, const CollisionComponent& component);
		static bool Deserialize(StreamReader* stream, CollisionComponent& component);
	};
}

