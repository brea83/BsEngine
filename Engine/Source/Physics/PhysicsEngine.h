#pragma once
#include "Scene/Entity.h"
//#include "Collider.h"

namespace Pixie
{
	
	struct CollisionEvent
	{
		CollisionEvent() = default;

		Entity A;
		Entity B;

		bool operator==(const CollisionEvent& other) const
		{
			return (A == other.A && B == other.B)
				|| (A == other.B && B == other.A);
		}

		bool operator!=(const CollisionEvent& other) const
		{
			return !(*this == other);
		}
	};

	class Collider;
	class SphereCollider;

	class PhysicsEngine
	{
	public:
		PhysicsEngine();
		void OnUpdate(Scene* scene, float deltaTime);
		//void UpdateEditor(Scene* scene, float deltatime)

		// when I have batching set up this will be where I batch the colliders and send them to the renderer
		//void DrawDebugColliders(Scene* scene);

		static bool CheckIntersect(Collider* colliderA, Collider* colliderB);
		const std::vector<CollisionEvent>& GetCollisions() const { return m_Collisions; }
		void ClearEvents() { m_Collisions.clear(); }
	private:
		std::vector<CollisionEvent> m_Collisions;

		static bool CheckSphereIntersect(SphereCollider* sphereA, Collider* colliderB);
	};
}

