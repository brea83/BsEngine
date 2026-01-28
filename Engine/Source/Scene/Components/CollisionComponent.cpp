#include "BsPrecompileHeader.h"
#include "CollisionComponent.h"

namespace Pixie
{
	const char* CollisionComponent::TypeNames[] = {
				"Sphere",
				"Plane",
				"Cube",
				"Mesh"
	};

	void CollisionComponent::on_construct(entt::registry& registry, const entt::entity entt)
	{
		Logger::Core(LOG_TRACE, "entt called CollisionComponent::on_construct");
	}

	void CollisionComponent::on_update(entt::registry & registry, const entt::entity entt)
	{
		CollisionComponent& component = registry.get<CollisionComponent>(entt);
		Logger::Core(LOG_TRACE, "entt called CollisionComponent::on_update");
		switch (component.Type)
		{
			case Pixie::ColliderType::Sphere:
			{
				Logger::Core(LOG_TRACE, "Adding SphereCollider to entity: {}", (uint32_t)entt);
				// add the correct type of collider
				registry.emplace_or_replace<SphereCollider>(entt);

				// remove any other types of colliders
				if (registry.any_of<PlaneCollider>(entt))
				{
					registry.remove<PlaneCollider>(entt);
					Logger::Core(LOG_TRACE, "Removed PlaneCollider from entity: {}", (uint32_t)entt);
				}

				if (registry.any_of<CubeCollider>(entt))
				{
					registry.remove<CubeCollider>(entt);
					Logger::Core(LOG_TRACE, "Removed CubeCollider from entity: {}", (uint32_t)entt);
				}

				if (registry.any_of<MeshCollider>(entt))
				{
					registry.remove<MeshCollider>(entt);
					Logger::Core(LOG_TRACE, "Removed MeshCollider from entity: {}", (uint32_t)entt);
				}

				break;
			}
			case Pixie::ColliderType::Plane:
			{
				// add the correct type of collider
				registry.emplace_or_replace<PlaneCollider>(entt);

				// remove any other types of colliders
				if (registry.any_of<SphereCollider>(entt))
				{
					registry.remove<SphereCollider>(entt);
				}

				if (registry.any_of<CubeCollider>(entt))
				{
					registry.remove<CubeCollider>(entt);
				}

				if (registry.any_of<MeshCollider>(entt))
				{
					registry.remove<MeshCollider>(entt);
				}
				break;
			}
			case Pixie::ColliderType::Cube:
			{
				// add the correct type of collider
				registry.emplace_or_replace<CubeCollider>(entt);

				// remove any other types of colliders
				if (registry.any_of<SphereCollider>(entt))
				{
					registry.remove<SphereCollider>(entt);
				}

				if (registry.any_of<PlaneCollider>(entt))
				{
					registry.remove<PlaneCollider>(entt);
				}

				if (registry.any_of<MeshCollider>(entt))
				{
					registry.remove<MeshCollider>(entt);
				}
				break;
			}

			case Pixie::ColliderType::Mesh:
			{
				// add the correct type of collider
				registry.emplace_or_replace<MeshCollider>(entt);

				// remove any other types of colliders
				if (registry.any_of<SphereCollider>(entt))
				{
					registry.remove<SphereCollider>(entt);
				}

				if (registry.any_of<PlaneCollider>(entt))
				{
					registry.remove<PlaneCollider>(entt);
				}

				if (registry.any_of<CubeCollider>(entt))
				{
					registry.remove<CubeCollider>(entt);
				}
				break;
			}
			default:
			{
				RemoveAllColliders(registry, entt);
				break;
			}
		}
	}
	void CollisionComponent::on_destroy(entt::registry & registry, const entt::entity entt)
	{
		Logger::Core(LOG_TRACE, "entt called CollisionComponent::on_destroy");
		RemoveAllColliders(registry, entt);
	}

	void CollisionComponent::RemoveAllColliders(entt::registry& registry, const entt::entity entt)
	{
		// remove any types of colliders
		if (registry.any_of<SphereCollider>(entt))
		{
			registry.remove<SphereCollider>(entt);
		}

		if (registry.any_of<PlaneCollider>(entt))
		{
			registry.remove<PlaneCollider>(entt);
		}

		if (registry.any_of<CubeCollider>(entt))
		{
			registry.remove<CubeCollider>(entt);
		}

		if (registry.any_of<MeshCollider>(entt))
		{
			registry.remove<MeshCollider>(entt);
		}
	}
}