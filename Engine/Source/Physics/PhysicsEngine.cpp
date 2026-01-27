#include "BsPrecompileHeader.h"
#include "PhysicsEngine.h"
#include "Scene/Components/Collider.h"
#include "Scene/Components/CollisionComponent.h"
#include "Scene/Components/Transform.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace Pixie
{
	PhysicsEngine::PhysicsEngine()
	{}
	
	void PhysicsEngine::Update(Scene* scene)
	{
		entt::registry& registry = scene->GetRegistry();
		auto view = registry.view<CollisionComponent>();

		for (auto entity : view)
		{
			CollisionComponent& component = view.get<CollisionComponent>(entity);

			if (component.Type == ColliderType::END || !component.BIsActive) continue;

			// add to octree
			
		}

	}
	
	void PhysicsEngine::DrawDebugColliders(Scene* scene)
	{}

	bool PhysicsEngine::CheckIntersect(Collider* colliderA, Collider* colliderB)
	{
		ColliderType typeA = colliderA->Type;
		
		switch (typeA)
		{
			case Pixie::ColliderType::Sphere:
			{
				auto* sphereCollider = static_cast<SphereCollider*>(colliderA);
				if (sphereCollider)
					return CheckSphereIntersect(sphereCollider, colliderB);
				else
					Logger::Core(LOG_ERROR, "Collider A had type == Sphere, but could not cast to SphereCollider"); // hopefully this never runs
				break;
			}
			case Pixie::ColliderType::Plane:
			{
				break;
			}
			case Pixie::ColliderType::Cube:
			{
				break;
			}
			case Pixie::ColliderType::AABB:
			{
				break;
			}
			case Pixie::ColliderType::Mesh:
			{
				break;
			}
			default:
			{
				Logger::Core(LOG_ERROR, "Collider A had an invalid Type"); // hopefully this never runs
				break;
			}
		}
		// add other types
		return false;
	}

	bool PhysicsEngine::CheckSphereIntersect(SphereCollider* sphereA, Collider* colliderB)
	{
		ColliderType typeB = colliderB->Type;

		switch (typeB)
		{
			case Pixie::ColliderType::Sphere:
			{
				auto* sphereB = static_cast<SphereCollider*>(colliderB);
				if (sphereB)
				{
					float r = sphereA->Radius + sphereB->Radius;

					// is the square distance between A and B < r squared? if so they intersect
					return glm::distance2(sphereA->Transform[3], sphereB->Transform[3]) < (r * r);
				}
				else
				{
					Logger::Core(LOG_ERROR, "Collider A had type == Sphere, but could not cast to SphereCollider"); // hopefully this never runs
				}
				break;
			}
			case Pixie::ColliderType::Plane:
			{
				break;
			}
			case Pixie::ColliderType::Cube:
			{
				break;
			}
			case Pixie::ColliderType::AABB:
			{
				break;
			}
			case Pixie::ColliderType::Mesh:
			{
				break;
			}
			default:
			{
				Logger::Core(LOG_ERROR, "Collider B had an invalid Type"); // hopefully this never runs
				break;
			}
		}
		return false;
	}
	

}