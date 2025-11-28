#include "BsPrecompileHeader.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

//

Transform::Transform(glm::vec3 position, glm::vec3 rotation , glm::vec3 scale)
    : _position(position), _eulerRotation(glm::radians(rotation)), 
    _orientation(glm::quat(glm::radians(rotation))), _scale(scale), 
    _localMatrix(glm::mat4(1.0f)), _worldMatrix(glm::mat4(1.0f))
{ }

void Transform::UnParent(bool bKeepWorldPosition)
{
    if (ParentTransform != nullptr)
    {
        glm::mat4 parentMatrix = ParentTransform->_localMatrix;

        _localMatrix = parentMatrix * _localMatrix;

        Decompose(_localMatrix, _scale, _orientation, _position );

        SetRotationQuaternion(_orientation, AngleType::Radians);

        if (ParentTransform->ParentTransform != nullptr)
        {
            ParentTransform = ParentTransform->ParentTransform;
            return;
        }
    }
    ParentTransform = nullptr;
}

glm::vec3 Transform::GetPosition()
{
    if(!_positionDirty) return _position;

    _localMatrix = glm::translate(_localMatrix, _position);
    _positionDirty = false;
    return _position;
}

void Transform::Rotate(float angle, glm::vec3 axis, AngleType angleType)
{
    if (angleType == AngleType::Degrees)
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

void Transform::SetRotationEuler(glm::vec3 value, AngleType angleType)
{
    if (angleType == AngleType::Degrees)
    {
        _eulerRotation = glm::radians(value);  
        _orientation = glm::quat(_eulerRotation); 
    }
    else
    {
        _eulerRotation = glm::vec3(value);
        _orientation = glm::quat(value);
    }

    _rotationDirty = true;
}

void Transform::SetRotationQuaternion(glm::quat orientation, AngleType angleType)
{
    if (angleType == AngleType::Degrees)
    {
        _orientation.w = glm::radians(orientation.w);
        _orientation.x = glm::radians(orientation.x);
        _orientation.y = glm::radians(orientation.y);
        _orientation.z = glm::radians(orientation.z);
        float yaw = glm::yaw(_orientation);
        float pitch = glm::pitch(_orientation);
        float roll = glm::roll(_orientation);

        _eulerRotation = glm::vec3(yaw, pitch, roll);
    }
    else
    {
        _orientation = orientation;
        float yaw =  glm::yaw(_orientation);
        float pitch = glm::pitch(_orientation);
        float roll = glm::roll(_orientation);

        _eulerRotation = glm::vec3(yaw, pitch, roll);
    }

    _rotationDirty = true;
}

glm::vec3 Transform::GetRotationEuler(AngleType angleType)
{
    if (_positionDirty || _scaleDirty || _rotationDirty)
    {
        RecalculateModelMatrix();
    }

    switch (angleType)
    {
    case AngleType::Degrees:
        return glm::degrees(_eulerRotation);
        break;
    case AngleType::Radians:
        return _eulerRotation;
        break;
    default:
        break;
    }
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

    _orientation = glm::normalize(_orientation);
    glm::mat4 rotation = glm::mat4_cast(_orientation);

   /* glm::mat4 rotation =
        glm::rotate(identity, glm::radians(_eulerRotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f })
        * glm::rotate(identity, glm::radians(_eulerRotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f })
        * glm::rotate(identity, glm::radians(_eulerRotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f });*/

    glm::mat4 scale = glm::scale(identity, _scale);

    _localMatrix = translation * rotation * scale;

    _positionDirty = false;
    _scaleDirty = false;
    _rotationDirty = false;
}

void Transform::Decompose(glm::mat4 const& modelMatrix, glm::vec3& scale, glm::quat& orientation, glm::vec3& translation)
{
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
}
