#include "BsPrecompileHeader.h"
#include "Collider.h"
#include "Transform.h"
namespace Pixie
{

	void SphereCollider::on_construct(entt::registry& registry, const entt::entity entt)
	{
		SphereCollider& collider = registry.get<SphereCollider>(entt);
		collider.Transform = registry.try_get<TransformComponent>(entt);
	}
	void SphereCollider::on_update(entt::registry& registry, const entt::entity entt)
	{ }

	void SphereCollider::on_destroy(entt::registry& registry, const entt::entity entt)
	{ }
}