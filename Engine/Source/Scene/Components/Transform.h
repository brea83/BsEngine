#pragma once
//#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <EnTT/entt.hpp>
#include "Scene/Components/Component.h"

namespace Pixie
{
	class Scene;

	enum class AngleType
	{
		Degrees,
		Radians
	};

	class TransformComponent
	{

	public:
		TransformComponent() = default;
		TransformComponent(glm::vec3 position, glm::vec3 rotation , glm::vec3 scale);
		TransformComponent(const TransformComponent&) = default;
		// properties
		static constexpr SerializableComponentID ID{ SerializableComponentID::TransformComponent };

		void UnParent(Scene* scene, entt::entity parent, entt::entity grandParent = entt::null, bool bKeepWorldPosition = true);
		entt::entity ParentEntityHandle{ entt::null };
		entt::entity EntityHandle{ entt::null };

		void SetPosition(glm::vec3 value) { m_Position = value; m_PositionDirty = true; }
		glm::vec3 GetPosition();
		const glm::vec3 ReadOnlyPosition() const { return m_Position; }

		glm::vec3 Forward() const;
		glm::vec3 Up() const;
		glm::vec3 Left() const;
		glm::vec3 Right() const;
		glm::vec3 Down() const;

		static void Decompose(glm::mat4 const& modelMatrix, glm::vec3& scale, glm::quat& orientation, glm::vec3& translation);
		static bool Decompose(glm::mat4 const& transform, glm::vec3& scale, glm::vec3& rotation, glm::vec3& translation);

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

		//operator overrides
		bool operator==(const TransformComponent& other) const
		{
			return EntityHandle == other.EntityHandle;
		}

		bool operator!=(const TransformComponent& other) const
		{
			return !(*this == other);
		}

		static void Serialize(StreamWriter* stream, const TransformComponent& component);
		static bool Deserialize(StreamReader* stream, TransformComponent& component);
	private:
		glm::vec3 m_Position { 0.0f };
		bool m_PositionDirty { true };

		//Note to self: store rotations as radians for easier import
		glm::vec3 m_EulerRotation { 0.0f };
		glm::quat m_Orientation{ 0.0f, 0.0f, 0.0f, 0.0f };
		bool m_RotationDirty { false };

		glm::vec3 m_Scale { 1.0f };
		bool m_ScaleDirty { false };
		
		void RecalculateModelMatrix();
		glm::mat4 m_LocalMatrix{ 1.0f };

		glm::mat4 m_WorldMatrix{ 1.0f };


		friend class DetailsViewPanel;
	};
}