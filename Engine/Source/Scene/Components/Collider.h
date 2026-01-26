#pragma once
#include <glm/glm.hpp>

namespace Pixie
{
	enum class ColliderType
	{
		Sphere,
		Plane,
		Cube,
		AABB,
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
		glm::mat4 Transform{ glm::mat4() };
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider() 
			: Collider(ColliderType::Sphere), Radius(0.5f) { }


		float Radius{ 0.5f };

	};
}