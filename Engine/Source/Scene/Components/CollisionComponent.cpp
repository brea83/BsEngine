#include "BsPrecompileHeader.h"
#include "CollisionComponent.h"
namespace Pixie
{
	const char* CollisionComponent::TypeNames[] = {
				"Sphere",
				"Plane",
				"Cube",
				"AABB",
				"Mesh"
	};

	void CollisionComponent::on_construct(entt::registry& registry, const entt::entity entt)
	{
		Logger::Log(LOG_TRACE, "entt called CollisionComponent::on_construct");
	}
	void CollisionComponent::on_update(entt::registry & registry, const entt::entity entt)
	{
		Logger::Log(LOG_TRACE, "entt called CollisionComponent::on_update");
	}
	void CollisionComponent::on_destroy(entt::registry & registry, const entt::entity entt)
	{
		Logger::Log(LOG_TRACE, "entt called CollisionComponent::on_destroy");
	}
}