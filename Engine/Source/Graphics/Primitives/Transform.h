#pragma once
//#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Pixie
{
	enum class AngleType
	{
		Degrees,
		Radians
	};

	class Transform
	{

	public:
		Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
		// properties
		void UnParent(bool bKeepWorldPosition = true);
		std::shared_ptr<Transform> ParentTransform;

		void SetPosition(glm::vec3 value) { m_Position = value; m_PositionDirty = true; }
		glm::vec3 GetPosition();

		glm::vec3 Forward() const;
		glm::vec3 Up() const;
		glm::vec3 Left() const;
		glm::vec3 Right() const;
		glm::vec3 Down() const;

		void Rotate(float angle, glm::vec3 axis, AngleType angleType = AngleType::Degrees);
		void SetRotationEuler(glm::vec3 value, AngleType angleType = AngleType::Degrees);
		void SetRotationQuaternion(glm::quat orientation, AngleType angleType = AngleType::Degrees);

		glm::vec3 GetRotationEuler(AngleType angleType = AngleType::Degrees);

		void SetScale(glm::vec3 value) { m_Scale = value; m_ScaleDirty = true; }
		glm::vec3 GetScale();

		// methods
		glm::mat4& GetObjectToWorldMatrix();

		glm::mat4 GetLocal() const { return m_LocalMatrix; }
		glm::mat4 GetWorld() const { return m_WorldMatrix; }

	private:
		glm::vec3 m_Position{ 0.0f };
		bool m_PositionDirty{ true };

		//Note to self: store rotations as radians for easier import
		glm::vec3 m_EulerRotation{ 0.0f };
		glm::quat m_Orientation;
		bool m_RotationDirty{ false };

		glm::vec3 m_Scale{ 1.0f };
		bool m_ScaleDirty{ false };

		void RecalculateModelMatrix();
		glm::mat4 m_LocalMatrix;

		glm::mat4 m_WorldMatrix;

		void Decompose(glm::mat4 const& modelMatrix, glm::vec3& scale, glm::quat& orientation, glm::vec3& translation);

		friend class DetailsViewPanel;
	};

}