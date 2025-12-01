#pragma once
#include "Component.h"
#include "Graphics/Camera.h"

class GameObject;

class CameraComponent : public Camera, public Component
{
public:
	CameraComponent(GameObject* parent) :_parentObject(parent), _name("Camera Component") {};
	~CameraComponent() {};
	// Inherited via Component
	void Initialize() override;
	// Inherited via Component
	std::string Name() const override { return _name; }
	void SetName(const std::string& name) override { _name = name; }

	void CleanUp() override;
	std::shared_ptr<Component> Clone() override;
	
	GameObject* GetParentObject() const override { return _parentObject; }
	void SetParentObject(GameObject* newParent) override;
	
	void OnUpdate() override;

	//virtual void LookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f)) override;
	virtual bool HandleMoveWasd(int keyCode, float deltaTime) override;
	virtual bool HandleLookMouse(float xOffset, float yOffset, float deltaTime) override;

	bool operator ==(const CameraComponent& other)
	{
		return Name() == other.Name()
			//TODO: ADD ID COMPARISSON
			&& _parentObject == other._parentObject;
	}

	protected:
		std::string _name;
		GameObject* _parentObject{ nullptr };


		// Inherited via Camera
		glm::mat4 ViewMatrix() const override;

};

