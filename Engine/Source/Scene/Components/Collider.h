#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Pixie
{
	enum class ColliderType
	{
		Sphere,
		Plane,
		Cube,
		Mesh,

		// end is always at the end and used for imgui visualizing stuff
		END 
	};

	class Collider
	{
	public:

		Collider() = default;
		Collider(ColliderType type) 
			: Type(type) { }

		ColliderType Type{ ColliderType::Sphere };
		glm::mat4 Transform{ glm::mat4(1.0f)};
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider() 
			: Collider(ColliderType::Sphere), Radius(0.5f) { }


		float Radius{ 0.5f };

	};

	class CubeCollider : public Collider
	{
	public:
		CubeCollider()
			: Collider(ColliderType::Cube), Extents(0.5f)
		{}

		// Extents are half width, height, depth in x, y, z
		glm::vec3 Extents{ 0.5f };

	};

	class PlaneCollider : public Collider
	{
	public:
		PlaneCollider()
			: Collider(ColliderType::Plane), Extents(0.5f)
		{}

		// Extents are half width, height, depth in x, y, z
		glm::vec3 Extents{ 0.5f };

	};

	class MeshCollider : public Collider
	{
	public:
		MeshCollider()
			: Collider(ColliderType::Mesh)/*, Extents(0.5f)*/
		{}
		// TODO: HOW DO?
		// Extents are half width, height, depth in x, y, z
		//glm::vec3 Extents{ 0.5f };

	};
}