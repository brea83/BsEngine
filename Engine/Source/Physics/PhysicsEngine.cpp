#include "BsPrecompileHeader.h"
#include "PhysicsEngine.h"
#include "Scene/Components/Collider.h"
#include "Scene/Components/CollisionComponent.h"
#include "Scene/Components/Transform.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/matrix_query.hpp>

namespace Pixie
{
	PhysicsEngine::PhysicsEngine()
	{}
	
	void PhysicsEngine::OnUpdate(Scene* scene, float deltaTime)
	{
		entt::registry& registry = scene->GetRegistry();
		auto view = registry.view<CollisionComponent, SphereCollider, TransformComponent>();

		for (auto&& [entity, collisonComp, collider, transform] : registry.view<CollisionComponent, SphereCollider, TransformComponent>().each())
		{
			if (collisonComp.Type == ColliderType::END || !collisonComp.BIsActive) continue;

			// TODO: add to octree
			
		}

	}
	
	//void PhysicsEngine::DrawDebugColliders(Scene* scene)
	//{}

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
		// need to check effective radius if the parent object has undergone a transform that should scale the collider too
		// this bit of math on the transform is extracting the scale of the X vector. and because we want spheres to stay uniform, we want to ignore the other axis scale
		float radiusA = sphereA->Radius * sphereA->Transform->GetLargestScaleComponent();

		switch (typeB)
		{
			case Pixie::ColliderType::Sphere:
			{
				auto* sphereB = static_cast<SphereCollider*>(colliderB);
				if (sphereB)
				{
					float radiusB = sphereB->Radius *sphereB->Transform->GetLargestScaleComponent();
					float r = radiusA + radiusB;

					// is the square distance between A and B < r squared? if so they intersect
					Logger::Core(LOG_DEBUG, "Collision found");;// ]) < (r * r);
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