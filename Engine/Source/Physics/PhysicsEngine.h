#pragma once
#include "Scene/Entity.h"
//#include "Collider.h"

namespace Pixie
{
	
	struct CollisionEvent
	{
		Entity A;
		Entity B;
	};

	class Collider;
	class SphereCollider;

	class PhysicsEngine
	{
	public:
		PhysicsEngine();
		void Update(Scene* scene);
		void DrawDebugColliders(Scene* scene);

		static bool CheckIntersect(Collider* colliderA, Collider* colliderB);
		const std::vector<CollisionEvent>& GetCollisions() const { return m_Collisions; }
		void ClearEvents() { m_Collisions.clear(); }
	private:
		std::vector<CollisionEvent> m_Collisions;

		static bool CheckSphereIntersect(SphereCollider* sphereA, Collider* colliderB);
	};
}

