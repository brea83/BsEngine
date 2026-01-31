#include "BsPrecompileHeader.h"
#include "Collider.h"
#include "Transform.h"
namespace Pixie
{
	//=========================
	// SPHERE COLLIDER
	//=========================
	void SphereCollider::on_construct(entt::registry& registry, const entt::entity entt)
	{
		SphereCollider& collider = registry.get<SphereCollider>(entt);
		collider.Transform = registry.try_get<TransformComponent>(entt);
	}
	void SphereCollider::on_update(entt::registry& registry, const entt::entity entt)
	{ }

	void SphereCollider::on_destroy(entt::registry& registry, const entt::entity entt)
	{ }
	void SphereCollider::Serialize(StreamWriter * stream, const SphereCollider & collider)
	{
		stream->WriteRaw<ColliderType>(collider.Type);
		stream->WriteRaw<bool>(collider.Colliding);
		stream->WriteRaw<float>(collider.Radius);
	}
	bool SphereCollider::Deserialize(StreamReader * stream, SphereCollider & collider)
	{
		stream->ReadRaw<ColliderType>(collider.Type);
		stream->ReadRaw<bool>(collider.Colliding);
		stream->ReadRaw<float>(collider.Radius);
		return true;
	}
	//=========================
	// CUBE COLLIDER
	//=========================
	void CubeCollider::on_construct(entt::registry& registry, const entt::entity entt)
	{
		CubeCollider& collider = registry.get<CubeCollider>(entt);
		collider.Transform = registry.try_get<TransformComponent>(entt);
	}
	void CubeCollider::on_update(entt::registry& registry, const entt::entity entt)
	{}

	void CubeCollider::on_destroy(entt::registry& registry, const entt::entity entt)
	{}
	void CubeCollider::Serialize(StreamWriter* stream, const CubeCollider& collider)
	{
		stream->WriteRaw<ColliderType>(collider.Type);
		stream->WriteRaw<bool>(collider.Colliding);
		stream->WriteRaw<glm::vec3>(collider.Extents);
	}
	bool CubeCollider::Deserialize(StreamReader * stream, CubeCollider & collider)
	{
		stream->ReadRaw<ColliderType>(collider.Type);
		stream->ReadRaw<bool>(collider.Colliding);
		stream->ReadRaw<glm::vec3>(collider.Extents);
		return true;
	}
	//=========================
	// PLANE COLLIDER
	//=========================
	void PlaneCollider::Serialize(StreamWriter* stream, const PlaneCollider& collider)
	{}
	bool PlaneCollider::Deserialize(StreamReader * stream, PlaneCollider & collider)
	{
		return false;
	}
	//=========================
	// MESH COLLIDER
	//=========================
	void MeshCollider::Serialize(StreamWriter* stream, const MeshCollider& collider)
	{}
	bool MeshCollider::Deserialize(StreamReader * stream, MeshCollider & collider)
	{
		return false;
	}
}