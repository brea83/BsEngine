#include "BsPrecompileHeader.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

//
namespace Pixie
{
    Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        : m_Position(position), m_EulerRotation(glm::radians(rotation)),
        m_Orientation(glm::quat(glm::radians(rotation))), m_Scale(scale),
        m_LocalMatrix(glm::mat4(1.0f)), m_WorldMatrix(glm::mat4(1.0f))
    {}

    void Transform::UnParent(bool bKeepWorldPosition)
    {
        if (ParentTransform != nullptr)
        {
            glm::mat4 parentMatrix = ParentTransform->m_LocalMatrix;

            m_LocalMatrix = parentMatrix * m_LocalMatrix;

            Decompose(m_LocalMatrix, m_Scale, m_Orientation, m_Position);

            SetRotationQuaternion(m_Orientation, AngleType::Radians);

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
        if (!m_PositionDirty) return m_Position;

        m_LocalMatrix = glm::translate(m_LocalMatrix, m_Position);
        m_PositionDirty = false;
        return m_Position;
    }

    glm::vec3 Transform::Forward() const
    {
        glm::vec3 direction;

        direction.x = cos(m_EulerRotation.x) * cos(m_EulerRotation.y);
        direction.y = sin(m_EulerRotation.y);
        direction.z = sin(m_EulerRotation.x) * cos(m_EulerRotation.y);

        return glm::normalize(direction);
    }

    glm::vec3 Transform::Up() const
    {
        return glm::normalize(glm::cross(Right(), Forward()));
    }

    glm::vec3 Transform::Left() const
    {
        return Right() * -1.0f;
    }

    glm::vec3 Transform::Right() const
    {
        return  glm::normalize(glm::cross(Forward(), glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    glm::vec3 Transform::Down() const
    {
        return Up() * -1.0f;
    }

    void Transform::Rotate(float angle, glm::vec3 axis, AngleType angleType)
    {
        if (angleType == AngleType::Degrees)
        {
            m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(angle), axis);
        }
        else
        {
            m_LocalMatrix = glm::rotate(m_LocalMatrix, angle, axis);
        }

        m_Orientation = glm::quat_cast(m_LocalMatrix);
        float yaw = glm::yaw(m_Orientation);
        float pitch = glm::pitch(m_Orientation);
        float roll = glm::roll(m_Orientation);

        m_EulerRotation = glm::vec3(yaw, pitch, roll);
        m_RotationDirty;
    }

    void Transform::SetRotationEuler(glm::vec3 value, AngleType angleType)
    {
        if (angleType == AngleType::Degrees)
        {
            m_EulerRotation = glm::radians(value);
            m_Orientation = glm::quat(m_EulerRotation);
        }
        else
        {
            m_EulerRotation = glm::vec3(value);
            m_Orientation = glm::quat(value);
        }

        m_RotationDirty = true;
    }

    void Transform::SetRotationQuaternion(glm::quat orientation, AngleType angleType)
    {
        if (angleType == AngleType::Degrees)
        {
            m_Orientation.w = glm::radians(orientation.w);
            m_Orientation.x = glm::radians(orientation.x);
            m_Orientation.y = glm::radians(orientation.y);
            m_Orientation.z = glm::radians(orientation.z);
            float yaw = glm::yaw(m_Orientation);
            float pitch = glm::pitch(m_Orientation);
            float roll = glm::roll(m_Orientation);

            m_EulerRotation = glm::vec3(yaw, pitch, roll);
        }
        else
        {
            m_Orientation = orientation;
            float yaw = glm::yaw(m_Orientation);
            float pitch = glm::pitch(m_Orientation);
            float roll = glm::roll(m_Orientation);

            m_EulerRotation = glm::vec3(yaw, pitch, roll);
        }

        m_RotationDirty = true;
    }

    glm::vec3 Transform::GetRotationEuler(AngleType angleType)
    {
        if (m_PositionDirty || m_ScaleDirty || m_RotationDirty)
        {
            RecalculateModelMatrix();
        }

        switch (angleType)
        {
        case AngleType::Degrees:
            return glm::degrees(m_EulerRotation);
            break;
        case AngleType::Radians:
            return m_EulerRotation;
            break;
        default:
            break;
        }
    }

    glm::vec3 Transform::GetScale()
    {
        if (m_PositionDirty || m_ScaleDirty || m_RotationDirty)
        {
            RecalculateModelMatrix();
        }

        return m_Scale;
    }

    glm::mat4& Transform::GetObjectToWorldMatrix()
    {
        if (m_PositionDirty || m_ScaleDirty || m_RotationDirty)
        {
            RecalculateModelMatrix();
        }

        if (ParentTransform != nullptr)
        {
            m_WorldMatrix = ParentTransform->GetObjectToWorldMatrix() * m_LocalMatrix;
            return m_WorldMatrix;
        }

        return m_LocalMatrix;
    }

    void Transform::RecalculateModelMatrix()
    {
        glm::mat4 identity = glm::mat4(1.0f);
        glm::mat4 translation = glm::translate(identity, m_Position);

        m_Orientation = glm::normalize(m_Orientation);
        glm::mat4 rotation = glm::mat4_cast(m_Orientation);

        /* glm::mat4 rotation =
             glm::rotate(identity, glm::radians(m_EulerRotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f })
             * glm::rotate(identity, glm::radians(m_EulerRotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f })
             * glm::rotate(identity, glm::radians(m_EulerRotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f });*/

        glm::mat4 scale = glm::scale(identity, m_Scale);

        m_LocalMatrix = translation * rotation * scale;

        m_PositionDirty = false;
        m_ScaleDirty = false;
        m_RotationDirty = false;
    }

    void Transform::Decompose(glm::mat4 const& modelMatrix, glm::vec3& scale, glm::quat& orientation, glm::vec3& translation)
    {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
    }
}