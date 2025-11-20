#pragma once
//#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
	std::shared_ptr<Transform> ParentTransform;

	void SetPosition(glm::vec3 value) { _position = value; _positionDirty = true; }
	glm::vec3 GetPosition();

	void Rotate(float angle, glm::vec3 axis, AngleType angleType = AngleType::Degrees);
	void SetRotationEuler(glm::vec3 value, AngleType angleType = AngleType::Degrees);
	void SetRotationQuaternion(glm::quat orientation, AngleType angleType = AngleType::Degrees);

	glm::vec3 GetRotationEuler(AngleType angleType = AngleType::Degrees);
	
	void SetScale(glm::vec3 value) { _scale = value; _scaleDirty = true; }
	glm::vec3 GetScale();
	
	// methods
	glm::mat4& GetObjectToWorldMatrix();


private:
	glm::vec3 _position { 0.0f };
	bool _positionDirty { false };

	//Note to self: store rotations as radians for easier import
	glm::vec3 _eulerRotation { 0.0f };
	glm::quat _orientation;
	bool _rotationDirty { false };

	glm::vec3 _scale { 1.0f };
	bool _scaleDirty { false };
	
	void RecalculateModelMatrix();
	glm::mat4 _localMatrix;

	glm::mat4 _worldMatrix;

	friend class DetailsViewPanel;
};

