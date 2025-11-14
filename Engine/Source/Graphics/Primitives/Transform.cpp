#include "BsPrecompileHeader.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

//

Transform::Transform()
{
    _localMatrix = glm::mat4(1.0f);
    _orientation = glm::quat(_eulerRotation);
}

glm::vec3 Transform::GetPosition()
{
    if(!_positionDirty) return _position;

    _localMatrix = glm::translate(_localMatrix, _position);
    _positionDirty = false;
    return _position;
}

void Transform::Rotate(float angle, glm::vec3 axis, bool isDegrees)
{
    if (isDegrees)
    {
        _localMatrix = glm::rotate(_localMatrix, glm::radians(angle), axis);
    }
    else
    {
        _localMatrix = glm::rotate(_localMatrix, angle, axis);
    }

    _orientation = glm::quat_cast(_localMatrix);
    float yaw = glm::yaw(_orientation);
    float pitch = glm::pitch(_orientation);
    float roll = glm::roll(_orientation);

    _eulerRotation = glm::vec3(yaw, pitch, roll);
    _rotationDirty;
}

void Transform::SetRotationEuler(glm::vec3 value, bool isDegrees)
{
    if (isDegrees)
    {
        _eulerRotation = value;
        _orientation = glm::quat(glm::radians(value)); 
    }
    else
    {
        _eulerRotation = glm::vec3(glm::degrees(value));
        _orientation = glm::quat(value);
    }

    _rotationDirty = true;
}

void Transform::SetRotationQuaternion(glm::quat orientation, bool isDegrees)
{
    if (isDegrees)
    {
        _orientation = orientation;
        float yaw = glm::yaw(_orientation);
        float pitch = glm::pitch(_orientation);
        float roll = glm::roll(_orientation);

        _eulerRotation = glm::vec3(yaw, pitch, roll);
    }
    else
    {
        _orientation = orientation;
        float yaw = glm::degrees( glm::yaw(_orientation));
        float pitch = glm::degrees(glm::pitch(_orientation));
        float roll = glm::degrees(glm::roll(_orientation));

        _eulerRotation = glm::vec3(yaw, pitch, roll);
    }

    _rotationDirty = true;
}

glm::vec3 Transform::GetRotationEuler()
{
    if (_positionDirty || _scaleDirty || _rotationDirty)
    {
        RecalculateModelMatrix();
    }

    return _eulerRotation;
}

glm::vec3 Transform::GetScale()
{
    if (_positionDirty || _scaleDirty || _rotationDirty)
    {
        RecalculateModelMatrix();
    }

    return _scale;
}

glm::mat4& Transform::GetObjectToWorldMatrix()
{
    if (_positionDirty || _scaleDirty || _rotationDirty)
    {
        RecalculateModelMatrix();
    } 

    if (ParentTransform != nullptr)
    {
        _worldMatrix = ParentTransform->GetObjectToWorldMatrix() * _localMatrix;
        return _worldMatrix;
    }

    return _localMatrix;
}

void Transform::RecalculateModelMatrix()
{
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 translation = glm::translate(identity, _position);

    //glm::mat4 rotation = glm::mat4_cast(glm::normalize(_orientation));
    glm::mat4 rotation =
        glm::rotate(identity, glm::radians(_eulerRotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f })
        * glm::rotate(identity, glm::radians(_eulerRotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f })
        * glm::rotate(identity, glm::radians(_eulerRotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f });

    glm::mat4 scale = glm::scale(identity, _scale);

    _localMatrix = translation * rotation * scale;

    _positionDirty = false;
    _scaleDirty = false;
    _rotationDirty = false;
}
