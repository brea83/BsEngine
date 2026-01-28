#include "BsPrecompileHeader.h"
#include "PhysicsEngine.h"
#include "Scene/Components/Collider.h"
#include "Scene/Components/CollisionComponent.h"
#include "Scene/Components/Transform.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/matrix_query.hpp>

namespace Pixie
{
	PhysicsEngine::PhysicsEngine()
	{}
	
	void PhysicsEngine::Update(Scene* scene)
	{
		entt::registry& registry = scene->GetRegistry();
		auto view = registry.view<CollisionComponent, SphereCollider, TransformComponent>();

		for (auto&& [entity, collisonComp, collider, transform] : registry.view<CollisionComponent, SphereCollider, TransformComponent>().each())
		{
			if (collisonComp.Type == ColliderType::END || !collisonComp.BIsActive) continue;
			
			if (glm::isIdentity(transform.GetObjectToWorldMatrix(), 0.01f) && glm::isIdentity(collider.Transform, 0.01f))
			{
				// matrices are identity and match don't need to sync collider transform
				continue;
			}

			// sync transform
			glm::vec3 center = transform.GetPosition();
			glm::quat orientation = transform.GetRotationQuaternion();
			glm::vec3 scale = transform.GetScale();

			scale *= collider.Radius;

			glm::mat4 identity = glm::mat4(1.0f);
			glm::mat4 translation = glm::translate(identity, center);
			glm::mat4 rotation = glm::mat4_cast(orientation);
			glm::mat4 scaleMat = glm::scale(identity, scale);

			collider.Transform = translation * rotation * scaleMat;

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