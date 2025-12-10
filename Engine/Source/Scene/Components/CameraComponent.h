#pragma once
#include "Core.h"
#include "Component.h"
#include "Graphics/Camera.h"


namespace Pixie
{
	class GameObject;

	class CameraComponent : public Camera, public Component
	{
	public:
		CameraComponent(GameObject* parent) :m_ParentObject(parent), m_Name("Camera Component") {};
		~CameraComponent() {};
		// Inherited via Component
		void Initialize() override;
		// Inherited via Component
		std::string Name() const override { return m_Name; }
		void SetName(const std::string& name) override { m_Name = name; }

		void CleanUp() override;
		std::shared_ptr<Component> Clone() override;

		GameObject* GetParentObject() const override { return m_ParentObject; }
		void SetParentObject(GameObject* newParent) override;

		void OnUpdate() override;

		//virtual void LookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f)) override;
		virtual bool HandleMoveWasd(int keyCode, float deltaTime) override;
		virtual bool HandleLookMouse(float xOffset, float yOffset, float deltaTime) override;

		bool operator ==(const CameraComponent& other)
		{
			return Name() == other.Name()
				//TODO: ADD ID COMPARISSON
				&& m_ParentObject == other.m_ParentObject;
		}

	protected:
		std::string m_Name;
		GameObject* m_ParentObject{ nullptr };


		// Inherited via Camera
		glm::mat4 ViewMatrix() const override;

	};

}