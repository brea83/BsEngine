#include "BsPrecompileHeader.h"
#include "Component.h"
#include "EngineContext.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "ImGui/ImGuiPanel.h"
#include "ScriptManager.h"

#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_relational.hpp>


//Component::Component(/*GameObject* parent, const std::string& name*/)
////: _parentObject(parent), Name(name)
//{}
namespace Pixie
{
    const char* LightComponent::LightTypeNames[] = {
                "Directional Light",
                "Point Light",
                "Spot Light"
    };

    // follow component

    const char* FollowComponent::TypeNames[(unsigned long long)SplineEndBehavior::END] = {
        "Stop",
        "Ping-Pong",
        "Teleport To Start",
    };

    glm::vec3 FollowComponent::HandleFollowing(float deltaTime, std::shared_ptr<Scene> scene, MovementComponent& moveComponent, glm::vec3 currentPosition)
    {
        GameObject target = scene->FindGameObjectByGUID(EntityToFollow);
        if (!target)
            return glm::vec3(0.0f);

        if (target.HasCompoenent<SplineComponent>() && FollowSplineIfAvailable)
        {
            SplineComponent& spline = target.GetComponent<SplineComponent>();
            return HandleSplineFollowing(deltaTime, spline, moveComponent, currentPosition);
        }
        else
        {
            glm::vec3 targetPos = target.GetTransform().GetPosition() + Offset;
            //float distance = glm::distance(targetPos, currentPosition);
            
            glm::vec3 vecToTarget = targetPos - currentPosition;
            float distance = glm::length(vecToTarget);
            if (distance <= MinDistFromTarget)
            {
                return glm::vec3(0.0f);
            }
            if (distance <= FollowThreshold && distance >= -FollowThreshold)
            {
                //moveComponent.Direction = glm::normalize(targetPos - currentPosition);
                //glm::vec3 movement = moveComponent.Speed * deltaTime * moveComponent.Direction;
                return glm::mix(glm::vec3(0.0f), vecToTarget, deltaTime * moveComponent.Speed);
            }
            //==========================================================
            //THESE CONDITIONS CAUSE BOUNCING AROUND THE TARGET POINT
            //if (vecToTarget.x >= 0.5f || vecToTarget.x <= -0.5f)
            //    moveComponent.Direction.x = vecToTarget.x;

            //if (vecToTarget.y >= 0.5f || vecToTarget.y <= -0.5f)
            //    moveComponent.Direction.y = vecToTarget.y;
            //    
            //if (vecToTarget.z >= 0.5f || vecToTarget.z <= -0.5f)
            //    moveComponent.Direction.z = vecToTarget.z;
            //moveComponent.Direction = glm::normalize(moveComponent.Direction);
            //return moveComponent.Speed * deltaTime * moveComponent.Direction;
            //==========================================================
                //return moveComponent.Speed * deltaTime * moveComponent.Direction; // use prev direction
            moveComponent.Direction = glm::normalize(targetPos - currentPosition);
            return moveComponent.Speed * deltaTime * moveComponent.Direction;
        }
    }

    // Todo: get other end behaviors implemented
    glm::vec3 FollowComponent::HandleSplineFollowing(float deltaTime, SplineComponent& spline, MovementComponent& moveComponent, glm::vec3 currentPosition)
    {
        if (FollowDirection == 0)
            return glm::vec3(0.0f);

        AccumulatedTime += deltaTime * moveComponent.Speed * FollowDirection;

        glm::vec3 splineAtT = spline.GetPostionT(AccumulatedTime);

        bool bStartIsEnd = FollowType == SplineEndBehavior::PingPong && FollowDirection < 0;
        glm::vec3 endPoint = bStartIsEnd ? glm::vec3(spline.Points.front()->GetWorld()[3]) : glm::vec3(spline.Points.back()->GetWorld()[3]);
        glm::bvec3 equality = glm::equal(splineAtT, endPoint);
        bool bIsEndOfSpline = equality.x && equality.y && equality.z;
        

        if (!bIsEndOfSpline)
        {
            glm::vec3 targetPos = splineAtT + Offset;
            return targetPos - currentPosition;
        }

        switch (FollowType)
        {
        case Pixie::SplineEndBehavior::Stop:
        {
            FollowDirection = 0;
            AccumulatedTime = 0.0f;
            break;
        }
        case Pixie::SplineEndBehavior::PingPong:
        {
            FollowDirection *= -1;
            //AccumulatedTime = 0.0f;
            break;
        }
        case Pixie::SplineEndBehavior::TeleportToStart:
        {
            FollowDirection = 1;
            AccumulatedTime = 0.0f;
            break;
        }
        default:
        {
            FollowDirection = 0;
            AccumulatedTime = 0.0f;
            break;
        }
        }
        return glm::vec3(0.0f);
    }

    InterpolatedTransform FollowComponent::AltFollowing(float deltaTime, std::shared_ptr<Scene> scene, MovementComponent& moveComponent, glm::vec3 currentPosition)
    {
        InterpolatedTransform result = InterpolatedTransform();

        GameObject target = scene->FindGameObjectByGUID(EntityToFollow);
        if (!target)
            return result;

        if (target.HasCompoenent<SplineComponent>() && FollowSplineIfAvailable)
        {
            SplineComponent& spline = target.GetComponent<SplineComponent>();
            return AltSplineFollowing(deltaTime, spline, moveComponent, currentPosition);
        }
        else
        {
            glm::vec3 targetPos = target.GetTransform().GetPosition() + Offset;
            //float distance = glm::distance(targetPos, currentPosition);

            glm::vec3 vecToTarget = targetPos - currentPosition;
            float distance = glm::length(vecToTarget);
            if (distance <= MinDistFromTarget)
            {
                return result;
            }
            if (distance <= FollowThreshold && distance >= -FollowThreshold)
            {
                result.Position = glm::mix(glm::vec3(0.0f), vecToTarget, deltaTime * moveComponent.Speed);
                return result;
            }
            moveComponent.Direction = glm::normalize(targetPos - currentPosition);
            result.Position = moveComponent.Speed * deltaTime * moveComponent.Direction;
            return result;
        }
    }

    InterpolatedTransform FollowComponent::AltSplineFollowing(float deltaTime, SplineComponent& spline, MovementComponent& moveComponent, glm::vec3 currentPosition)
    {
        InterpolatedTransform result = InterpolatedTransform();

        if (FollowDirection == 0)
            return result;

        bool bStartIsEnd = FollowType == SplineEndBehavior::PingPong && FollowDirection < 0;
        
        bool bEndPointReached = false; 
        if (bStartIsEnd)
            bEndPointReached = AccumulatedTime <= 0.0f;
        else
            bEndPointReached = spline.IsTAtOrPastEndPoint(AccumulatedTime);

        // accumulate time for next loop 
        AccumulatedTime += deltaTime * moveComponent.Speed * FollowDirection;

        if (!bEndPointReached)
        {
            glm::mat4 splineAtT = spline.GetTransformAtT(AccumulatedTime);

            glm::vec3 scale;
            //glm::quat orientation;
            //glm::vec3 translation;
            TransformComponent::Decompose(splineAtT, scale, result.Orientation, result.Position);

           // result.Orientation = orientation;
           // result.Position = translation + Offset;
            result.Position = result.Position + Offset - currentPosition;
            return result;
        }

        switch (FollowType)
        {
        case Pixie::SplineEndBehavior::Stop:
        {
            FollowDirection = 0;
            AccumulatedTime = 0.0f;
            break;
        }
        case Pixie::SplineEndBehavior::PingPong:
        {
            FollowDirection *= -1;
            //AccumulatedTime = 0.0f;
            break;
        }
        case Pixie::SplineEndBehavior::TeleportToStart:
        {
            FollowDirection = 1;
            AccumulatedTime = 0.0f;
            break;
        }
        default:
        {
            FollowDirection = 0;
            AccumulatedTime = 0.0f;
            break;
        }
        }

        return result;
    }

    void FollowComponent::on_construct(entt::registry& registry, const entt::entity entt)
    {
        HasUpdateableComponents* updateableComponent = registry.try_get<HasUpdateableComponents>(entt);
        if (updateableComponent)
            return;

        registry.emplace<HasUpdateableComponents>(entt);
    }

    void FollowComponent::on_destroy(entt::registry & registry, const entt::entity entt)
    {
        //check for other updatable components, if none remove the tag.
        MovementComponent* component = registry.try_get<MovementComponent>(entt);
        if (component)
            return;

        registry.remove<HasUpdateableComponents>(entt);
    }

    // movement constraints
    glm::vec3 MovementConstraintsComponent::ConstrainOnFrustum(GameObject& object, glm::vec3 currentWorldPos, glm::vec3& moveAmount)
    {
        std::shared_ptr<Scene> scene = object.GetScene();

        glm::vec4 newPosition = glm::vec4(currentWorldPos + moveAmount, 1.0f);
        //position in Frustum-Space
        CameraManager& camManager = scene->GetCameraManager();
        GameObject camObject = camManager.GetActiveCameraObject();
        CameraComponent& cam = camObject.GetComponent<CameraComponent>();
        TransformComponent& camTransform = camObject.GetTransform();

        glm::mat4 storedFrustum{ 1.0f };
        auto& frustumMap = camManager.GetFrustums();
        if (frustumMap.find(CameraID) != frustumMap.end())
        {
            storedFrustum = frustumMap.at(CameraID).GetMatrix();
        }
        glm::mat4 frustum = cam.Cam.ProjectionMatrix() * glm::inverse(camTransform.GetModelMatrix());

        glm::vec4 posInFS = frustum * newPosition;
        posInFS = posInFS / posInFS.w;

        if ((posInFS.x > -1.0f * posInFS.w && posInFS.x < posInFS.w)
            && (posInFS.y > -1.0f * posInFS.w && posInFS.y < posInFS.w)
            && (posInFS.z > -1.0f * posInFS.w && posInFS.z < posInFS.w))
        {
            return moveAmount;
        }

        //Logger::Game(LOG_DEBUG, "{} is at local pos ({},{},{}) and global pos ({},{},{})", object.GetName(), localPos.x,localPos.y,localPos.z, globalPos.x, globalPos.y, globalPos.z);
        //outside near || far planes
        if (posInFS.z <= -1.0f * posInFS.w || posInFS.z >= posInFS.w)
            return glm::vec3(0.0f);
        //outside left
        if (posInFS.x <= -1.0f * posInFS.w)
            moveAmount.x += posInFS.x + posInFS.w;
        //outside right
        if (posInFS.x >= posInFS.w)
            moveAmount.x += posInFS.x - posInFS.w;
        //outside bottom
        if (posInFS.y <= -1.0f * posInFS.w)
            moveAmount.y -= posInFS.y + posInFS.w;
        //outside top
        if (posInFS.y >= posInFS.w)
            moveAmount.y -= posInFS.y - posInFS.w;

        return moveAmount;
    }

    glm::vec3 MovementConstraintsComponent::ConstrainMoveAmount(GameObject& object, TransformComponent& transform, glm::vec3& moveAmount)
    {
        
        glm::vec4 position = glm::vec4(transform.GetPosition(), 1.0f);

        if (BUseCamFrustum)
        {
            return ConstrainOnFrustum(object, transform.GetModelMatrix()[3], moveAmount);
        }
        else
        {
            //TODO implement other movement constraints
            return moveAmount;
        }

        
    }

    // movement component 

    void MovementComponent::on_construct(entt::registry& registry, const entt::entity entt)
    {
        HasUpdateableComponents* component = registry.try_get<HasUpdateableComponents>(entt);
        if (component)
            return;

        registry.emplace<HasUpdateableComponents>(entt);
    }
    void MovementComponent::on_destroy(entt::registry & registry, const entt::entity entt)
    {
        //check for other updatable components, if none remove the tag.
        FollowComponent* component = registry.try_get<FollowComponent>(entt);
        if (component)
            return;

        registry.remove<HasUpdateableComponents>(entt);
    }

    // orbit component

    void OrbitComponent::on_construct(entt::registry& registry, const entt::entity entt)
    {
        MovementComponent* movement = registry.try_get<MovementComponent>(entt);
        if (movement == nullptr)
        {
            registry.emplace<MovementComponent>(entt);
            return;
        }

        HasUpdateableComponents* updateable = registry.try_get<HasUpdateableComponents>(entt);
        if (updateable)
            return;
        else
            registry.emplace<HasUpdateableComponents>(entt);

    }

    // Native Scripting Component
    
    void NativeScriptComponent::AttachScript(const std::string& name, GameObject& destinationObject)
    {
        ScriptManager* scripts = ScriptManager::GetInstance();
        StoredScript newScript;

        if (scripts->FindStoredScript(name, newScript))
        {
            AttachedScriptNames.push_back(name);
            AttachedScripts[AttachedScriptNames.back()] = newScript;
            AttachedScripts[name].AttachComponent(destinationObject);
        }

    }

    void NativeScriptComponent::Serialize(StreamWriter * stream, const GameObject& sourceObject, const NativeScriptComponent & component)
    {
        stream->WriteArray<std::string>(component.AttachedScriptNames);

        for (int i = 0; i < component.AttachedScriptNames.size(); i++)
        {
            std::string name = component.AttachedScriptNames[i];
            component.AttachedScripts.at(name).Serialize(stream, sourceObject);
        }

    }

    bool NativeScriptComponent::Deserialize(StreamReader * stream, GameObject& destinationObject, NativeScriptComponent& component)
    {
        component.AttachedScriptNames.clear();
        std::vector<std::string> names;
        stream->ReadArray<std::string>(names);

        for (auto name : names)
        {
            component.AttachScript(name, destinationObject);

            component.AttachedScripts[name].Deserialize(stream, destinationObject);
        }

        return true;
    }

}