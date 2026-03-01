#include "BsPrecompileHeader.h"
#include "Scene/Components/Component.h"
#include "ImGuiPanel.h"
#include "ScriptManager.h"

namespace Pixie
{

    // Tag Component ------------------
    void TagComponent::Draw(GameObject& selected)
    {
        static std::string editingName = Tag;

        ImGuiPanel::DrawStringProperty("Tag", Tag, editingName);
    }

    // Transform Component -----------------

    void Pixie::TransformComponent::Draw(GameObject& selected)
    {
        ImGuiPanel::SliderParams params;
        params.Speed = 0.01f;
        if (ImGuiPanel::DrawVec3Control("Position", m_Position, params))
        {
            m_PositionDirty = true;
        }

        //translate rotation from radians to degrees
        glm::vec3 eulerDegrees = GetRotationEuler();
        if (ImGuiPanel::DrawVec3Control("Rotation", eulerDegrees, params))
        {
            SetRotationEuler(eulerDegrees);
        }

        params.ResetValue = 1.0f;
        if (ImGuiPanel::DrawVec3Control("Scale", m_Scale, params))
        {
            m_ScaleDirty = true;
        }

        ImGui::TextWrapped("There is a known issue with ImGuizmo's Rotation gizmo:");
        ImGui::TextWrapped("If the camera forward vector is paralel to one of the gizmo circle planes those handles will not behave.");
    }

    // Mesh Component ----------------------
    void Pixie::MeshComponent::Draw(GameObject& selected)
    {
        std::string previousMeshPath = m_FilePath;


        if (ImGuiPanel::FileProperty("Mesh File", m_FilePath, "All Formats (*.fbx, *.obj)\0*.fbx;*.obj\0FBX Model (*.fbx)\0*.fbx\0OBJ Mesh(*.obj)\0*.obj\0"))
        {
            if (!AssetLoader::LoadMesh(selected, *this, m_FilePath))
            {
                Logger::Core(LOG_WARNING, "DetailsViewPanel::DrawComponents() MESH FILE There was an error loading mesh file, reverting to old mesh path");
                m_FilePath = previousMeshPath;
                Reload();
            }
        }

        ImGui::SeparatorText("Material Instance");
        MaterialInstance& material = m_MaterialInstance;

        std::string previousBasePath = material.BaseMapPath;

        if (ImGuiPanel::FileProperty("Base Texture", material.BaseMapPath,
            "All Formats (*.png, *.jpeg, *.jpg)\0*.png;*.jpeg;*.jpg\0png (*.png)\0*.png\0Jpeg (*.jpeg)\0*.jpeg\0Jpg (*.jpg)\0*.jpg\0"))
        {
            std::shared_ptr<Texture> newTexture = AssetLoader::LoadTexture(material.BaseMapPath);
            if (newTexture == nullptr)
            {
                Logger::Core(LOG_WARNING, "DetailsViewPanel::DrawComponents() BASE TEXTURE There was an error loading Texture file, reverting to old Texture path");
                material.BaseMapPath = previousBasePath;
            }
            else
            {
                m_MaterialInstance.BaseMap = newTexture;
            }
        }

        ImGui::SameLine();
        ImGui::PushID("RemoveColorTexture");
        if (ImGui::Button("X"))
        {
            material.BaseMap = nullptr;
            material.BaseMapPath = "";
        }
        ImGui::PopID();

        ImGui::Text("Base Color");
        ImGui::SameLine();
        ImGui::ColorEdit3("##Color", glm::value_ptr(material.BaseColor));

        std::string previousNormalPath = material.NormalMapPath;

        if (ImGuiPanel::FileProperty("Normal Map", material.NormalMapPath,
            "All Formats (*.png, *.jpeg, *.jpg)\0*.png;*.jpeg;*.jpg\0png (*.png)\0*.png\0Jpeg (*.jpeg)\0*.jpeg\0Jpg (*.jpg)\0*.jpg\0"))
        {
            std::shared_ptr<Texture> newTexture = AssetLoader::LoadTexture(material.NormalMapPath, TextureType::Normal);
            if (newTexture == nullptr)
            {
                Logger::Core(LOG_WARNING, "DetailsViewPanel::DrawComponents() NORMAL TEXTURE There was an error loading Texture file, reverting to old Texture path");
                material.NormalMapPath = previousBasePath;
            }
            else
            {
                m_MaterialInstance.NormalMap = newTexture;
            }
        }

        ImGui::SameLine();
        ImGui::PushID("RemoveNormalMap");
        if (ImGui::Button("X"))
        {
            material.NormalMap = nullptr;
            material.NormalMapPath = "";
        }
        ImGui::PopID();

        std::string previousMetalPath = material.MetallicMapPath;


        if (ImGuiPanel::FileProperty("Metallic Map", material.MetallicMapPath,
            "All Formats (*.png, *.jpeg, *.jpg)\0*.png;*.jpeg;*.jpg\0png (*.png)\0*.png\0Jpeg (*.jpeg)\0*.jpeg\0Jpg (*.jpg)\0*.jpg\0"))
        {
            std::shared_ptr<Texture> newTexture = AssetLoader::LoadTexture(material.MetallicMapPath, TextureType::GltfMetalicRoughness);
            if (newTexture == nullptr)
            {
                Logger::Core(LOG_WARNING, "DetailsViewPanel::DrawComponents() METALLIC TEXTURE There was an error loading Texture file, reverting to old Texture path");
                material.MetallicMapPath = previousMetalPath;
            }
            else
            {
                m_MaterialInstance.MetallicMap = newTexture;
            }
        }

        ImGui::SameLine();
        ImGui::PushID("RemoveMetalMap");
        if (ImGui::Button("X"))
        {
            material.MetallicMap = nullptr;
            material.MetallicMapPath = "";
        }
        ImGui::PopID();

        ImGuiPanel::SliderParams smoothnessParams;
        smoothnessParams.Min = 0.0f;
        smoothnessParams.Max = 1.0f;
        smoothnessParams.Speed = 0.001f;
        smoothnessParams.ResetValue = 0.3f;
        ImGuiPanel::DrawFloatControl("Smoothness", material.Smoothness, smoothnessParams);

        ImGuiPanel::SliderParams specParams;
        specParams.Min = 1.0f;
        specParams.Max = 200.0f;
        specParams.Speed = 0.1f;
        specParams.ResetValue = 32.0f;
        ImGuiPanel::DrawFloatControl("Specular Power", material.SpecularPower, specParams);
    }

    // Circle Renderer Component ---------------
    void Pixie::CircleRendererComponent::Draw(GameObject& selected)
    {
        ImGui::ColorEdit4("Color", glm::value_ptr(Color));
        ImGui::DragFloat("Line Width", &LineWidth, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Fade", &Fade, 0.00025f, 0.0f, 1.0f);
    }

    // Collision Component

    void Pixie::CollisionComponent::Draw(GameObject& selected)
    {
        std::shared_ptr<Scene> scene = selected.GetScene();

        ImGui::Text("Collider Type");
        ImGui::SameLine();
        int currentType = static_cast<int>(Type);
        if (ImGui::Combo("##ColliderType", &currentType, CollisionComponent::TypeNames, IM_ARRAYSIZE(CollisionComponent::TypeNames)))
        {
            ColliderType newType = static_cast<ColliderType>(currentType);

            if (newType != Type)
            {
                Type = newType;
                scene->GetRegistry().patch<CollisionComponent>(selected.GetEnttHandle());
            }
        }
        ImGui::Checkbox("Active", &BIsActive);


        // specific colliders

        if (selected.HasCompoenent<SphereCollider>())
        {
            SphereCollider& collider = selected.GetComponent<SphereCollider>();
            ImGuiPanel::SliderParams params;
            params.ResetValue = 0.5f;
            params.Speed = 0.001f;
            params.Min = 0.0f;

            ImGuiPanel::DrawFloatControl("Radius", collider.Radius, params);
        }

        if (selected.HasCompoenent<CubeCollider>())
        {
            CubeCollider& collider = selected.GetComponent<CubeCollider>();
            ImGuiPanel::SliderParams params;
            params.ResetValue = 0.5f;
            params.Speed = 0.001f;
            params.Min = 0.0f;

            ImGuiPanel::DrawVec3Control("Extents", collider.Extents, params);
            ImGui::SetItemTooltip("Extents are the HALF width/height/depth values");
        }
    }

    // Light Component ---------------------
    void Pixie::LightComponent::Draw(GameObject& selected)
    {
        ImGui::Text("Light Type");
        ImGui::SameLine();
        int currentType = static_cast<int>(Type);
        if (ImGui::Combo("##Light Type", &currentType, LightComponent::LightTypeNames, IM_ARRAYSIZE(LightComponent::LightTypeNames)))
        {
            Type = static_cast<LightType>(currentType);
        }

        ImGui::Text("Light Color");
        ImGui::SameLine();
        ImGui::ColorEdit3("##Color", glm::value_ptr(Color));

        ImGuiPanel::SliderParams attenuationParams;
        attenuationParams.Speed = 0.001f;
        attenuationParams.Format = "%.4f";
        if (Type == LightType::Point)
        {
            ImGuiPanel::DrawVec3Control("Attenuations", Attenuation, attenuationParams);

        }

        if (Type == LightType::Spot)
        {
            //DrawVec3Control("Direction", light.Direction, 0.5f);
            ImGuiPanel::DrawVec3Control("Attenuations", Attenuation, attenuationParams);

            ImGuiPanel::SliderParams params;
            params.Min = 1.0f;
            params.Max = 180.0f;
            params.ResetValue = 12.5f;
            params.Speed = 0.01f;
            ImGuiPanel::DrawFloatControl("Inner Radius", InnerRadius, params);

            ImGuiPanel::SliderParams paramsOuter;
            paramsOuter.Min = 1.0f;
            paramsOuter.Max = 180.0f;
            paramsOuter.ResetValue = 15.0f;
            paramsOuter.Speed = 0.01f;
            ImGuiPanel::DrawFloatControl("Outer Radius", OuterRadius, paramsOuter);
        }
    }

    // Player Input Component --------------------------
    void Pixie::PlayerInputComponent::Draw(GameObject& selected)
    {
        std::string guid = std::to_string(PlayerGUID);
        std::string enttID = std::to_string((uint32_t)PlayerEnttID);

        ImGui::BeginDisabled();
        ImGuiPanel::DrawStringProperty("Player GUID", guid, guid);
        ImGuiPanel::DrawStringProperty("Player Entt ID", enttID, enttID);
        ImGui::EndDisabled();
    }

    // Movement Constraints -------------------
    void Pixie::MovementConstraintsComponent::Draw(GameObject& selected)
    {
        std::shared_ptr<Scene> scene = selected.GetScene();

        ImGui::Text("Use Camera Frustum as Bounds");
        ImGui::SameLine();
        ImGui::BeginDisabled();
        ImGui::Checkbox("##isFrustum", &BUseCamFrustum);
        ImGui::EndDisabled();

        if (BUseCamFrustum)
        {
            BConstraintOnGlobalPosition = true;
        }
        else
        {
            ImGui::Text("Constrain on Global Position");
            ImGui::SameLine();
            ImGui::Checkbox("##isGlobal", &BConstraintOnGlobalPosition);
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("false == only constrained on local position");
            }
        }

        ImGuiPanel::SliderParams params;
        params.ResetValue = 1.0f;
        params.Speed = 0.01f;
        if (BUseCamFrustum)
        {
            ImGuiPanel::DrawVec3Control("Minimum Position", MinPosition, params);
            ImGuiPanel::DrawVec3Control("Maximum Position", MaxPosition, params);
        }
        else
        {
            if (CameraID == 0)
            {
                GameObject defaultCam = scene->GetCameraManager().GetDefaultCamera();
                if (defaultCam)
                    CameraID = defaultCam.GetGUID();
            }
            GameObject target = scene->FindGameObjectByGUID(CameraID);
            std::string targetString = target ? target.GetName() : " ";
            ImGui::SeparatorText("Camera to use");
            ImGui::BeginDisabled();
            ImGuiPanel::DrawStringProperty("##Target", targetString, targetString);
            // maybe renable selecting which cam to constrain to later
            // for now auto constraining to the active/default cam seems best for player constraint
            /*if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HEIRARCHY_ITEM"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(GameObject));
                    GameObject droppedObject = *(const GameObject*)payload->Data;
                    CameraComponent* cam = droppedObject.TryGetComponent<CameraComponent>();

                    if (cam)
                    {
                        glm::mat4 projection = cam->Cam.ProjectionMatrix();
                        glm::mat4 view = glm::inverse(droppedObject.GetTransform().GetObjectToWorldMatrix());
                        component.CameraID = droppedObject.GetGUID();
                        component.FrustumMatrix =Frustum::CalcFrustumMatrix(projection, view);
                    }
                }
                ImGui::EndDragDropTarget();
            }*/
            ImGui::EndDisabled();
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("will automatically constrain to active cam at runtime, or default cam at edit time");
            }
        }

    }

    // Movement Component
    void Pixie::MovementComponent::Draw(GameObject& selected)
    {
        ImGuiPanel::SliderParams params;
        params.ResetValue = 1.0f;
        params.Speed = 0.01f;

        ImGuiPanel::DrawFloatControl("Speed", Speed, params);

        ImGui::BeginDisabled();
        glm::vec3 direction = Direction;
        ImGuiPanel::DrawVec3Control("Direction", direction, params);
        ImGui::EndDisabled();
    }

    // Follow Component ---------------------
    void Pixie::FollowComponent::Draw(GameObject& selected)
    {
        std::shared_ptr<Scene> scene = selected.GetScene();

        ImGuiPanel::SliderParams params;
        params.Speed = 0.001f;
        params.ResetValue = 0.0f;
        ImGuiPanel::DrawVec3Control("Offset", Offset, params);
        ImGuiPanel::DrawFloatControl("Rounding Error", FollowThreshold, params);

        GameObject target = scene->FindGameObjectByGUID(EntityToFollow);
        std::string targetString = target ? target.GetName() : "";

        ImGui::SeparatorText("Target to Follow");
        ImGui::BeginDisabled();
        ImGuiPanel::DrawStringProperty("##Target", targetString, targetString);
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HEIRARCHY_ITEM"))
            {
                IM_ASSERT(payload->DataSize == sizeof(GameObject));
                GameObject droppedObject = *(const GameObject*)payload->Data;

                EntityToFollow = droppedObject.GetGUID();
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::EndDisabled();
        ImGui::Separator();

        ImGui::Checkbox("Follow Spline if Available", &FollowSplineIfAvailable);

        if (FollowSplineIfAvailable)
        {
            ImGui::Text("End Of Spline Behavior");
            ImGui::SameLine();
            int currentType = static_cast<int>(FollowType);

            if (ImGui::Combo("##ColliderType", &currentType, FollowComponent::TypeNames, IM_ARRAYSIZE(FollowComponent::TypeNames)))
            {
                FollowType = static_cast<SplineEndBehavior>(currentType);
            }
        }
    }

    // Orbit Component ---------------

    void Pixie::OrbitComponent::Draw(GameObject& selected)
    {
        ImGuiPanel::SliderParams params;
        params.Speed = 0.001f;
        params.ResetValue = 0.0f;
        ImGuiPanel::DrawVec3Control("Orbital Center", Origin, params);
        if (!selected.HasCompoenent<FollowComponent>() && ImGui::BeginDragDropTarget())
        {
            //seems handy to drag and drop origins based on other objects even when not explicitly following those objects
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HEIRARCHY_ITEM"))
            {
                IM_ASSERT(payload->DataSize == sizeof(GameObject));
                GameObject droppedObject = *(const GameObject*)payload->Data;

                Origin = droppedObject.GetTransform().GetPosition();
            }
            ImGui::EndDragDropTarget();
        }
        if (ImGui::BeginItemTooltip())
        {
            ImGui::Text("Can DragDrop from heirarchy to copy a transform pos to origin");
            ImGui::EndTooltip();
        }
        ImGuiPanel::DrawFloatControl("Radius", Radius, params);
        ImGuiPanel::DrawFloatControl("Start Angle Offset", StartingAngle, params);
    } 

    // Spline Component -----------------------

    void Pixie::SplineComponent::Draw(GameObject& selected)
    {
        ImGui::Text("Type");
        ImGui::SameLine();
        int currentType = static_cast<int>(GetType());
        
        if (ImGui::Combo("##ColliderType", &currentType, SplineComponent::TypeNames, IM_ARRAYSIZE(SplineComponent::TypeNames)))
        {
            SetType(static_cast<SplineType>(currentType));
        }
        /*ImGui::Text("Is Loop ");
        ImGui::SameLine();
        ImGui::Checkbox("##isLoop", &component.IsLoop);*/


        float previewTime = PreviewTime;
        float maxTime = GetNumSegments() + 1.0f;
        ImGuiPanel::SliderParams params;
        params.Min = 0.0f;
        params.Max = maxTime;
        params.Speed = 0.01f;

        ImGuiPanel::DrawFloatControl("Preview T", PreviewTime, params);

        ImGui::Text("Debug Color");
        ImGui::SameLine();
        ImGui::ColorEdit3("##Color", glm::value_ptr(DebugColor));

        ImGui::Text("Segments ");
        ImGui::SameLine();
        int oldSegmentCount = GetNumSegments();
        int newSegmentCount = oldSegmentCount;
        if (ImGui::InputInt("##segmentCount", &newSegmentCount))
        {
            if (newSegmentCount > oldSegmentCount)
                AddSegment(selected);
            if (newSegmentCount < oldSegmentCount)
                RemoveSegment(selected);
        }
    }

    // Native Script Component --------------
    void NativeScriptComponent::Draw(GameObject& selected)
    {
        ScriptManager* scripts = ScriptManager::GetInstance();
        ImGui::Text("ScriptComponents");
        ImGui::SameLine();
        std::vector<std::string> componentScripts = scripts->GetScriptNames();

        static int currentScriptIndex = 0;
        const char* combo_preview_value = componentScripts[currentScriptIndex].c_str();
        ImGui::PushItemWidth(ImGui::CalcTextSize("A Really Long Script").x);
        if (ImGui::BeginCombo("##scriptsPicker", combo_preview_value))
        {
            for (int i = 0; i < componentScripts.size(); i++)
            {
                const bool is_selected = (currentScriptIndex == i);
                if (ImGui::Selectable(componentScripts[i].c_str(), is_selected))
                    currentScriptIndex = i;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        if (currentScriptIndex == 0)
            ImGui::BeginDisabled();
        ImGui::SameLine();
        if (ImGui::Button("ADD"))
        {
            std::string name = componentScripts[currentScriptIndex];

            if (AttachedScripts.find(name) != AttachedScripts.end())
            {
                Logger::Core(LOG_DEBUG, "script named {} is already attached to {}", name, selected.GetName());
            }
            else
            {
                AttachScript(name, selected);

            }
        }
        if (currentScriptIndex == 0)
            ImGui::EndDisabled();
        std::vector<std::string> funcsToRemove;
        //std::string buttonText = "Remove";
        //ImVec2 buttonSize{ ImGui::CalcTextSize(buttonText.c_str()).x + (ImGui::GetStyle().FramePadding.x * 2.0f), ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };


        ImGuiChildFlags flags =  ImGuiChildFlags_ResizeY;
        for (size_t i = 0; i < AttachedScriptNames.size(); i++)
        {
       
            bool bDoNotDeleteHeader = true;
            std::string name = AttachedScriptNames[i];
            ImGui::PushID(name.c_str());
            //ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - buttonSize.x);
            
            //if (override_bg_color)
            ImGui::SetCursorPosX(ImGui::GetTreeNodeToLabelSpacing());
           // if (override_bg_color)

            ImGui::PushStyleColor( ImGuiCol_WindowBg, IM_COL32(20, 0, 20, 100));
            if (ImGui::CollapsingHeader(name.c_str(), &bDoNotDeleteHeader, ImGuiTreeNodeFlags_DefaultOpen))
            {
                float offset = ImGui::GetTreeNodeToLabelSpacing();
                ImGui::SetCursorPosX(offset);
                ImGui::BeginChild((name + "child").c_str(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), flags, ImGuiWindowFlags_None);
                ImGui::PopStyleColor();

                //if(DrawScriptFunctions.find(name) != DrawScriptFunctions.end())
                if (AttachedScripts.find(name) != AttachedScripts.end() && AttachedScripts[name].Draw)
                    AttachedScripts[name].Draw(selected);

                ImGui::EndChild();
            }
            else
            {
                ImGui::PopStyleColor();
            }


            if (!bDoNotDeleteHeader)
                funcsToRemove.push_back(name);
            //ImGui::PopItemWidth();
            //ImGui::SameLine();
            //if (ImGui::Button("Remove Script", buttonSize))
            //{
            //    funcsToRemove.push_back(name);
            //}
            ImGui::PopID();
        }

        for (auto key : funcsToRemove)
        {
            auto attachedScriptName = std::find(AttachedScriptNames.begin(), AttachedScriptNames.end(), key);
            if (attachedScriptName != AttachedScriptNames.end())
                AttachedScriptNames.erase(attachedScriptName);

            if (AttachedScripts.find(key) != AttachedScripts.end())
                AttachedScripts.erase(key);
        }
    }
}