#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
{
    _modelMatrix = glm::mat4(1.0f);
}

glm::vec3 Transform::GetPosition()
{
    if(!_positionDirty) return _position;

    _modelMatrix = glm::translate(_modelMatrix, _position);
    _positionDirty = false;
    return _position;
}

void Transform::Rotate(float angle, glm::vec3 axis, bool isDegrees)
{
    if (isDegrees)
    {
        _modelMatrix = glm::rotate(_modelMatrix, glm::radians(angle), axis);
    }
    else
    {
        _modelMatrix = glm::rotate(_modelMatrix, angle, axis);
    }
}

//glm::vec3 Transform::GetRotationEuler()
//{
//    return glm::vec3();
//}

glm::vec3 Transform::GetScale()
{
    return glm::vec3();
}

glm::mat4& Transform::GetObjectToWorldMatrix()
{
    if (_positionDirty)
    {
        _modelMatrix = glm::translate(_modelMatrix, _position);
        _positionDirty = false;
    } 
    
    if (_scaleDirty)
    {
        _modelMatrix = glm::scale(_modelMatrix, _scale);
        _scaleDirty = false;
    }

    return _modelMatrix;
}
