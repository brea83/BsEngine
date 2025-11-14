#pragma once
//#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{

public:
	Transform();
	// properties
	Transform* ParentTransform{ nullptr };

	void SetPosition(glm::vec3 value) { _position = value; _positionDirty = true; }
	glm::vec3 GetPosition();

	void Rotate(float angle, glm::vec3 axis, bool isDegrees = true);
	void SetRotationEuler(glm::vec3 value, bool isDegrees = true);
	void SetRotationQuaternion(glm::quat orientation, bool isDegrees = true);

	glm::vec3 GetRotationEuler();
	
	void SetScale(glm::vec3 value) { _scale = value; _scaleDirty = true; }
	glm::vec3 GetScale();
	
	// methods
	glm::mat4& GetObjectToWorldMatrix();


private:
	glm::vec3 _position { 0.0f };
	bool _positionDirty { false };

	glm::vec3 _eulerRotation { 0.0f };
	glm::quat _orientation;
	bool _rotationDirty { false };

	glm::vec3 _scale { 1.0f };
	bool _scaleDirty { false };
	
	void RecalculateModelMatrix();
	glm::mat4 _localMatrix;

	glm::mat4 _worldMatrix;

	friend class SceneHierarchyPanel;
};

