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

glm::vec3 Transform::GetRotationEuler()
{
    return glm::vec3();
}

glm::vec3 Transform::GetScale()
{
    return glm::vec3();
}

glm::mat4& Transform::GetObjectToWorldMatrix()
{
    if (_positionDirty || _scaleDirty || _rotationDirty)
    {
        RecalculateModelMatrix();
    } 

    return _localMatrix;
}

void Transform::RecalculateModelMatrix()
{
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 translation = glm::translate(identity, _position);

    glm::mat4 rotation = glm::mat4_cast(glm::normalize(_orientation));

    glm::mat4 scale = glm::scale(identity, _scale);

    _localMatrix = translation * rotation * scale;
    _positionDirty = false;
    _scaleDirty = false;
    _rotationDirty = false;
}
