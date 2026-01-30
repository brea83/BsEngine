#include "BsPrecompileHeader.h"
#include "PhysicsEngine.h"
#include "Scene/Components/Collider.h"
#include "Scene/Components/CollisionComponent.h"
#include "Scene/Components/Transform.h"
#include "DataStructures/KDTree.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_relational.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/matrix_query.hpp>

namespace Pixie
{
	using KDTreeVec3 = Data::KDTreeVec3;
	using KDNode = Data::KDNode;

	PhysicsEngine::PhysicsEngine()
	{}
	
	void PhysicsEngine::OnUpdate(Scene* scene, float deltaTime)
	{
		m_Collisions.clear();

		entt::registry& registry = scene->GetRegistry();
		//auto view = registry.view<CollisionComponent, SphereCollider, TransformComponent>();

		std::vector<entt::entity> entityList;
		std::vector<glm::vec3> positions;
		float largestRadius = FLT_MIN;

		for (auto&& [entity, collisonComp, transform] : registry.view<CollisionComponent, TransformComponent>().each())
		{
			if (collisonComp.Type == ColliderType::END || !collisonComp.BIsActive) continue;

			//add to list to turn into kd Tree
			entityList.emplace_back(entity);
			positions.emplace_back(transform.GetPosition());

			SphereCollider* sphere = registry.try_get<SphereCollider>(entity);
			if (sphere)
			{
				largestRadius = glm::max(largestRadius, sphere->Radius);
			}
		}

		if (positions.empty()) return; // early out don't do physics check because there are no physics enabled components
		// KDNodes preserve original index position as Node.Index so we can use that to lookup the entity
		KDTreeVec3 kdTree = KDTreeVec3(positions);
		

		std::vector<CollisionEvent> collisionPairsBroadPhase;

		for (auto&& [entity, sphere, transform] : registry.view<SphereCollider, TransformComponent>().each())
		{
			// use largest radius to make search range of kdTree
			std::vector<KDNode*> nodesInRange = kdTree.FindNodesInRange(transform.GetPosition(), sphere.Radius + largestRadius);

			for (KDNode* node : nodesInRange)
			{
				if (entityList[node->Index] == entity)
				{
					// do not make a pair with self
					continue;
				}
				CollisionEvent pair = CollisionEvent{ Entity(entity, scene), Entity(entityList[node->Index], scene) };

				if (collisionPairsBroadPhase.empty() || std::find(collisionPairsBroadPhase.begin(), collisionPairsBroadPhase.end(), pair) != collisionPairsBroadPhase.end())
				{
					collisionPairsBroadPhase.push_back(pair);
				}
			}
		}

		// narrow phase
		for (CollisionEvent pair : collisionPairsBroadPhase)
		{
			SphereCollider* sphereA = pair.A.TryGetComponent<SphereCollider>();
			SphereCollider* sphereB = pair.B.TryGetComponent<SphereCollider>();
			
			if (CheckIntersect(sphereA, sphereB))
			{
				m_Collisions.push_back(pair);
			}

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