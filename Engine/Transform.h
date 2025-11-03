#pragma once
//#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{

public:
	Transform();
	// properties
	void SetPosition(glm::vec3 value) { _position = value; _positionDirty = true; }
	glm::vec3 GetPosition();

	void Rotate(float angle, glm::vec3 axis, bool isDegrees = true);
	void SetRotationEuler(glm::vec3 value) { _eulerRotation = value; _orientation = glm::quat(glm::radians(value)); _rotationDirty = true; }
	glm::vec3 GetRotationEuler();
	
	void SetScale(glm::vec3 value) { _scale = value; _scaleDirty = true; }
	glm::vec3 GetScale();
	
	// methods
	glm::mat4& GetObjectToWorldMatrix();


private:
	glm::vec3 _position { 0.0f, 0.0f, 0.0f };
	bool _positionDirty { false };

	glm::vec3 _eulerRotation { 0.0f, 0.0f, 0.0f };
	glm::quat _orientation;
	bool _rotationDirty { false };

	glm::vec3 _scale { 1.0f, 1.0f, 1.0f };
	bool _scaleDirty { false };
	
	void RecalculateModelMatrix();
	glm::mat4 _modelMatrix;
};

