#include "BsPrecompileHeader.h"
#include "DetailsViewPanel.h"
#include "Pixie.h"

#include "CameraManager.h"
#include "ImGui/ImGuiPanel.h"
#include "Resources/AssetLoader.h"
#include "PlatformUtils.h"
#include "Source/ScriptManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>

namespace Pixie
{
	bool DetailsViewPanel::Draw(std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> selected)
	{

		ImGui::Begin("Details View");
		{
			if (scene == nullptr || selected == nullptr || selected->GetScene() == nullptr)
			{
				ImGui::End();
				return false;
			}
			entt::registry& registry = scene->GetRegistry();
			if (!registry.valid(*selected))
			{
				ImGui::End();
				return false;
			}

			NameComponent& nameComp = selected->GetComponent<NameComponent>();
			static std::string editingName = nameComp.Name;


			ImGuiPanel::DrawStringProperty("Name", nameComp.Name, editingName);

			if (ImGui::BeginPopupContextItem("AddComponentPopUp"))
			{
				if (ImGui::Selectable("Script Component"))
				{
					selected->AddComponent<NativeScriptComponent>();
				}

				if (ImGui::Selectable("Tag Component"))
				{
					selected->AddComponent<TagComponent>();
				}

				ImGui::Separator();

				if (ImGui::Selectable("Mesh Component"))
				{
					selected->AddComponent<MeshComponent>();
				}

				if (ImGui::Selectable("Circle Renderer"))
				{
					selected->AddComponent<CircleRendererComponent>();
				}

				ImGui::Separator();

				if (ImGui::Selectable("Camera Component"))
				{
					selected->AddComponent<CameraComponent>();
					CameraController& controller = selected->AddComponent<CameraController>();
					//controller.UpdateFocalPoint(selected);
				}
				if (ImGui::Selectable("Light"))
				{
					selected->AddComponent<LightComponent>();
				}

				if (ImGui::Selectable("Collision component"))
				{
					selected->AddComponent<CollisionComponent>();
				}

				if (ImGui::Selectable("Spline component"))
				{
					selected->AddComponent<SplineComponent>();
				}

				ImGui::Separator();

				if (ImGui::Selectable("Player Input component"))
				{
					selected->AddComponent<PlayerInputComponent>();
				}

				if (ImGui::Selectable("Movement component"))
				{
					selected->AddComponent<MovementComponent>();
				}

				if (selected->HasCompoenent<MovementComponent>())
				{
					if (ImGui::Selectable("Movement constraints") )
					{
						selected->AddComponent<MovementConstraintsComponent>();
					}
				}

				if (ImGui::Selectable("FollowComponent"))
				{
					selected->AddComponent<FollowComponent>();
				}

				if (ImGui::Selectable("Orbit Component"))
				{
					selected->AddComponent<OrbitComponent>();
				}

				ImGui::EndPopup();
			}
			if (ImGui::Button("AddComponent"))
			{
				ImGui::OpenPopup("AddComponentPopUp");
			}

			IDComponent* id = selected->TryGetComponent<IDComponent>();
			
			std::string guid = "0";
			if(id)
				guid = std::to_string((uint64_t)id->ID);

			std::string enttID = std::to_string((uint32_t)selected->GetEnttHandle());

			ImGui::BeginDisabled();
			ImGuiPanel::DrawStringProperty("Player GUID", guid, guid);
			ImGuiPanel::DrawStringProperty("Player Entt ID", enttID, enttID);
			ImGui::EndDisabled();

			ImGui::SeparatorText("Componenets");
			DrawComponents(scene, *selected/*selectedObject->GetAllComponents()*/);

		}
		ImGui::End();
		return true;
	}

	bool DetailsViewPanel::ItterateThroughComponents(std::shared_ptr<Scene> scene, GameObject& selected)
	{
		for (auto&& [id, type] : entt::resolve())
		{
			Logger::Core(LOG_INFO, "------------------------------");
			Logger::Core(LOG_DEBUG, "Found metadata for: {}", type.name());

			for (auto&& [id, metaData] : type.data())
			{
				Logger::Core(LOG_DEBUG, "{}: {}", metaData.type().info().name(), metaData.name());
				auto info = metaData.type().info();
			}

			for (auto&& [id, func] : type.func())
			{
				Logger::Core(LOG_TRACE, "{}: returns : {}", func.name(), func.ret().info().name());
				Logger::Core(LOG_TRACE, "accepts {} arguments", func.arity());

				for (int i = 0; i < func.arity(); i++)
				{
					Logger::Core(LOG_TRACE, "{}", func.arg(i).info().name());
				}
			}
			
		}
		return false;
	}

	void DetailsViewPanel::DrawComponents(std::shared_ptr<Scene> scene, GameObject& selected)
	{
		//ItterateThroughComponents(scene, selected);
		entt::registry& registry = scene->GetRegistry();
		if (selected.HasCompoenent<TransformComponent>())
		{
			ImGui::SeparatorText("Transform");

			TransformComponent& transform = selected.GetTransform();
			
			ImGuiPanel::SliderParams params;
			params.Speed = 0.01f;
			if (ImGuiPanel::DrawVec3Control("Position", transform.m_Position, params))
			{
				transform.m_PositionDirty = true;
			}

			//translate rotation from radians to degrees
			glm::vec3 eulerDegrees = transform.GetRotationEuler();
			if (ImGuiPanel::DrawVec3Control("Rotation", eulerDegrees, params))
			{
				transform.SetRotationEuler(eulerDegrees);
			}

			params.ResetValue = 1.0f;
			if (ImGuiPanel::DrawVec3Control("Scale", transform.m_Scale, params))
			{
				transform.m_ScaleDirty = true;
			}

			ImGui::TextWrapped("There is a known issue with ImGuizmo's Rotation gizmo:");
			ImGui::TextWrapped("If the camera forward vector is paralel to one of the gizmo circle planes those handles will not behave.");
		}

		if (selected.HasCompoenent<TagComponent>())
		{
			if (ImGui::CollapsingHeader("Tag Component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				bool removeTag = false;
				ImVec2 buttonSize{ ImGui::GetContentRegionAvail().x, ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };
				if (ImGui::Button("Remove Tag", buttonSize))
				{
					removeTag = true;
				}
				TagComponent& component = selected.GetComponent<TagComponent>();
				static std::string editingName = component.Tag;


				ImGuiPanel::DrawStringProperty("Tag", component.Tag, editingName);

				if (removeTag)
				{
					selected.RemoveComponent<TagComponent>();
				}
			}
		}

		if (selected.HasCompoenent<SplineComponent>())
		{
			ImGui::PushID("Spline");
			ImGui::Separator();
			SplineComponent& component = selected.GetComponent<SplineComponent>();
			ImGui::Text("Slpine Component");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			ImGui::Text("Type");
			ImGui::SameLine();
			int currentType = static_cast<int>(component.GetType());
			if (ImGui::Combo("##ColliderType", &currentType, SplineComponent::TypeNames, IM_ARRAYSIZE(SplineComponent::TypeNames)))
			{
				component.SetType( static_cast<SplineType>(currentType));
			}

			/*ImGui::Text("Is Loop ");
			ImGui::SameLine();
			ImGui::Checkbox("##isLoop", &component.IsLoop);*/


			float previewTime = component.PreviewTime;
			float maxTime = component.GetNumSegments() + 1.0f;
			ImGuiPanel::SliderParams params;
			params.Min = 0.0f;
			params.Max = maxTime;
			params.Speed = 0.01f;

			ImGuiPanel::DrawFloatControl("Preview T", component.PreviewTime, params);

			ImGui::Text("Debug Color");
			ImGui::SameLine();
			ImGui::ColorEdit3("##Color", glm::value_ptr(component.DebugColor));

			ImGui::Text("Segments ");
			ImGui::SameLine();
			int oldSegmentCount = component.GetNumSegments();
			int newSegmentCount = oldSegmentCount;
			if (ImGui::InputInt("##segmentCount", &newSegmentCount))
			{
				if (newSegmentCount > oldSegmentCount)
					component.AddSegment(selected);
				if (newSegmentCount < oldSegmentCount)
					component.RemoveSegment(selected);
			}

			ImGui::PopID();

			if (removeComponent)
			{
				selected.RemoveComponent<SplineComponent>();
			}
		}

		if (selected.HasCompoenent<PlayerInputComponent>())
		{
			ImGui::PushID("PlayerInput");
			ImGui::Separator();
			PlayerInputComponent& component = selected.GetComponent<PlayerInputComponent>();
			ImGui::Text("Input Component");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			std::string guid = std::to_string(component.PlayerGUID);
			std::string enttID = std::to_string((uint32_t)component.PlayerEnttID);

			ImGui::BeginDisabled();
			ImGuiPanel::DrawStringProperty("Player GUID", guid, guid);
			ImGuiPanel::DrawStringProperty("Player Entt ID", enttID, enttID);
			ImGui::EndDisabled();

			ImGui::PopID();

			if (removeComponent)
			{
				selected.RemoveComponent<PlayerInputComponent>();
			}
		}

		if (selected.HasCompoenent<MovementComponent>())
		{
			ImGui::PushID("MoveComponent");
			ImGui::Separator();
			MovementComponent& component = selected.GetComponent<MovementComponent>();
			ImGui::Text("Movement Component");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			ImGuiPanel::SliderParams params;
			params.ResetValue = 1.0f;
			params.Speed = 0.01f;

			ImGuiPanel::DrawFloatControl("Speed", component.Speed, params);

			ImGui::BeginDisabled();
			glm::vec3 direction = component.Direction;
			ImGuiPanel::DrawVec3Control("Direction", direction, params);
			ImGui::EndDisabled();

			ImGui::PopID();

			if (removeComponent)
			{
				selected.RemoveComponent<MovementComponent>();
			}
		}

		if (selected.HasCompoenent<MovementConstraintsComponent>())
		{
			ImGui::PushID("MoveConstraints");
			ImGui::Separator();
			MovementConstraintsComponent& component = selected.GetComponent<MovementConstraintsComponent>();
			ImGui::Text("Movement Constraints");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			ImGui::Text("Use Camera Frustum as Bounds");
			ImGui::SameLine();
			ImGui::Checkbox("##isFrustum", &component.BUseCamFrustum);

			if (component.BUseCamFrustum)
			{
				component.BConstraintOnGlobalPosition = true;
			}
			else
			{
				ImGui::Text("Constrain on Global Position");
				ImGui::SameLine();
				ImGui::Checkbox("##isGlobal", &component.BConstraintOnGlobalPosition);
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("false == only constrained on local position");
				}
			}

			ImGuiPanel::SliderParams params;
			params.ResetValue = 1.0f;
			params.Speed = 0.01f;
			if (!component.BUseCamFrustum)
			{
				ImGuiPanel::DrawVec3Control("Minimum Position", component.MinPosition, params);
				ImGuiPanel::DrawVec3Control("Maximum Position", component.MaxPosition, params);
			}			
			else
			{
				if (component.CameraID == 0)
				{
					GameObject defaultCam = scene->GetCameraManager().GetDefaultCamera();
					if (defaultCam)
						component.CameraID = defaultCam.GetGUID();
				}
				GameObject target = scene->FindGameObjectByGUID(component.CameraID);
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

			ImGui::PopID();

			if (removeComponent)
			{
				selected.RemoveComponent<MovementConstraintsComponent>();
			}
		}

		if (selected.HasCompoenent<FollowComponent>())
		{
			ImGui::PushID("FollowerComponent");
			ImGui::Separator();
			FollowComponent& component = selected.GetComponent<FollowComponent>();
			ImGui::Text("Follower Component");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			ImGuiPanel::SliderParams params;
			params.Speed = 0.001f;
			params.ResetValue = 0.0f;
			ImGuiPanel::DrawVec3Control("Offset", component.Offset, params);
			ImGuiPanel::DrawFloatControl("Rounding Error", component.FollowThreshold, params);

			GameObject target = scene->FindGameObjectByGUID(component.EntityToFollow);
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

					component.EntityToFollow = droppedObject.GetGUID();
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::EndDisabled();
			ImGui::Separator();

			ImGui::Checkbox("Follow Spline if Available", &component.FollowSplineIfAvailable);

			if (component.FollowSplineIfAvailable)
			{
				ImGui::Text("End Of Spline Behavior");
				ImGui::SameLine();
				int currentType = static_cast<int>(component.FollowType);

				if (ImGui::Combo("##ColliderType", &currentType, FollowComponent::TypeNames, IM_ARRAYSIZE(FollowComponent::TypeNames)))
				{
					component.FollowType = static_cast<SplineEndBehavior>(currentType);
				}
			}


			ImGui::PopID();

			if (removeComponent)
			{
				selected.RemoveComponent<FollowComponent>();
			}
		}

		if (selected.HasCompoenent<OrbitComponent>())
		{
			ImGui::PushID("OrbitComponent");
			ImGui::Separator();
			OrbitComponent& component = selected.GetComponent<OrbitComponent>();
			ImGui::Text("Orbit Component");
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Currently only stores data about an orbit, move component is used to move along an orbit");
			}
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			ImGuiPanel::SliderParams params;
			params.Speed = 0.001f;
			params.ResetValue = 0.0f;
			ImGuiPanel::DrawVec3Control("Orbital Center", component.Origin, params);
			if (!selected.HasCompoenent<FollowComponent>() && ImGui::BeginDragDropTarget())
			{
				//seems handy to drag and drop origins based on other objects even when not explicitly following those objects
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HEIRARCHY_ITEM"))
				{
					IM_ASSERT(payload->DataSize == sizeof(GameObject));
					GameObject droppedObject = *(const GameObject*)payload->Data;

					component.Origin = droppedObject.GetTransform().GetPosition();
				}
				ImGui::EndDragDropTarget();
			}
			ImGuiPanel::DrawFloatControl("Radius", component.Radius, params);

			ImGui::PopID();

			if (removeComponent)
			{
				selected.RemoveComponent<OrbitComponent>();
			}
		}

		if (selected.HasCompoenent<NativeScriptComponent>())
		{
			ImGui::PushID("NativeScriptComponent");
			ImGui::Separator();
			NativeScriptComponent& component = selected.GetComponent<NativeScriptComponent>();
			ImGui::Text("Script Component");
			
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			component.DrawComponent(selected);

			ImGui::PopID();

			if (removeComponent)
			{
				selected.RemoveComponent<NativeScriptComponent>();
			}
		}

		if (selected.HasCompoenent<MeshComponent>())
		{
			ImGui::PushID("MeshComponent");
			ImGui::Separator();
			MeshComponent& component = selected.GetComponent<MeshComponent>();
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), component.Name().c_str());
			ImGui::Text(buffer);
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			std::string previousMeshPath = component.m_FilePath;


			if (ImGuiPanel::FileProperty("Mesh File", component.m_FilePath, "All Formats (*.fbx, *.obj)\0*.fbx;*.obj\0FBX Model (*.fbx)\0*.fbx\0OBJ Mesh(*.obj)\0*.obj\0"))
			{
				if (!AssetLoader::LoadMesh(selected, component, component.m_FilePath))
				{
					Logger::Core(LOG_WARNING, "DetailsViewPanel::DrawComponents() MESH FILE There was an error loading mesh file, reverting to old mesh path");
					component.m_FilePath = previousMeshPath;
					component.Reload();
				}
			}

			ImGui::SeparatorText("Material Instance");
			MaterialInstance& material = component.m_MaterialInstance;

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
					component.m_MaterialInstance.BaseMap = newTexture;
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
					component.m_MaterialInstance.NormalMap = newTexture;
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
					component.m_MaterialInstance.MetallicMap = newTexture;
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

			if (removeComponent)
			{
				selected.RemoveComponent<MeshComponent>();
			}
			ImGui::PopID();
		}

		if (selected.HasCompoenent<CircleRendererComponent>())
		{
			ImGui::PushID("CircleRenderer");
			CircleRendererComponent& component = selected.GetComponent<CircleRendererComponent>();
			ImGui::Text("Circle Renderer Component");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Line Width", &component.LineWidth, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);

			ImGui::PopID();
		}

		if (selected.HasCompoenent<CollisionComponent>())
		{
			ImGui::PushID("CollisionComponent");
			ImGui::Separator();
			CollisionComponent& component = selected.GetComponent<CollisionComponent>();
			ImGui::Text("Collision Component");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			ImGui::Text("Collider Type");
			ImGui::SameLine();
			int currentType = static_cast<int>(component.Type);
			if (ImGui::Combo("##ColliderType", &currentType, CollisionComponent::TypeNames, IM_ARRAYSIZE(CollisionComponent::TypeNames)))
			{
				ColliderType newType = static_cast<ColliderType>(currentType);

				if (newType != component.Type)
				{
					component.Type = newType;
					scene->GetRegistry().patch<CollisionComponent>(selected.GetEnttHandle());
				}
			}
			ImGui::Checkbox("Active", &component.BIsActive);


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

			if (removeComponent)
			{
				selected.RemoveComponent<CollisionComponent>();
			}

			ImGui::PopID();
		}

		if (selected.HasCompoenent<CameraComponent>())
		{
			CameraManager& camManager = scene->GetCameraManager();
			ImGui::PushID("CameraComponent");
			ImGui::Separator();
			CameraComponent& component = selected.GetComponent<CameraComponent>();
			
			ImGui::Text("Camera Component");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();

			if (camManager.GetActiveCameraObject() != selected)
			{
				if (ImGui::Button("Possess"))
				{
					camManager.SetActiveCamera(selected);
				}
			}
			else
			{
				if (ImGui::Button("Un Possess"))
				{
					camManager.SetEditorCamActive();
				}
			}
		
			std::string buttonText = camManager.GetDefaultCamera() != selected ? "Make Default" : "Is Default";
			float textWidth = ImGui::CalcTextSize(buttonText.c_str()).x + (ImGui::GetStyle().FramePadding.x * 2.0f);
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - textWidth);
			if (camManager.GetDefaultCamera() != selected)
			{
				if (ImGui::Button(buttonText.c_str()))
				{
					camManager.SetDefaultCamera(selected);
				}
			}
			else
			{
				ImGui::BeginDisabled();
				ImGui::Button(buttonText.c_str());
				ImGui::EndDisabled();
			}

			ImGui::SeparatorText("ShadowMap Debug");

			if (ImGui::Button("Try Match Light Position"))
			{
				TransformComponent& transform = selected.GetTransform();
				GameObject mainLight = selected.GetScene()->GetMainLight();
				LightComponent* light = mainLight.TryGetComponent<LightComponent>();

				if (light != nullptr)
				{
					TransformComponent& lightTransform = mainLight.GetTransform();
					glm::vec3 forward = lightTransform.Forward();
					glm::vec3 forwardDegrees = glm::degrees(forward);

					transform.SetPosition(forward * -1.0f);
					transform.SetRotationEuler(lightTransform.GetRotationEuler());

					//glm::vec3 rotationDegrees = transform.GetRotationEuler();
				}

			}

			ImGui::Separator();

			ImGui::Text("Projection Type");
			ImGui::SameLine();
			const char* projectionTypeNames[] = { "Perspective", "Orthographic" };
			int bOrthographic = (int)component.IsOrthographic();

			if (ImGui::Combo("##Projection Type", &bOrthographic, projectionTypeNames, IM_ARRAYSIZE(projectionTypeNames)))
			{
				component.SetOrthographic(bOrthographic == 1);
			}

			if (component.IsOrthographic())
			{
				DrawOrthographicCamProps(component.Cam);
			}
			else
			{
				DrawPerspectiveCamProps(component.Cam);
			}

			

			if (removeComponent)
			{
				selected.RemoveComponent<CameraComponent>();
			}
			ImGui::PopID();
		}

		if (selected.HasCompoenent<CameraController>())
		{
			ImGui::PushID("CameraController");
			ImGui::Separator();
			CameraController& component = selected.GetComponent<CameraController>();

			ImGui::Text("Camera Controller");
			/*ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}
			ImGui::SetItemTooltip("Cannot Remove Controller via ui at this time");*/
			ImGui::Separator();

			ImGui::Text("Fly In Editor Only ");
			ImGui::SameLine();
			bool bEditorOnly = component.IsEditorOnly();
			if(ImGui::Checkbox("##IsEditorOnly", &bEditorOnly))
			{
				component.SetEditorOnly(bEditorOnly);
			}

			ImGui::Text("Focal Point");
			ImGui::SameLine();
			glm::vec3 focalPoint = component.GetFocalPoint();
			float focalDistance = component.GetFocalPointDistance();

			ImGui::BeginDisabled();
			ImGui::InputFloat3("##readOnly", glm::value_ptr(focalPoint));

			ImGui::Text("Distance to Focal Point");
			ImGui::InputFloat("##readOnlyDistance", &focalDistance);

			ImGui::EndDisabled();

			ImGui::PopID();
		}

		if (selected.HasCompoenent<LightComponent>())
		{
			LightComponent& light = selected.GetComponent<LightComponent>();
			ImGui::PushID("LightComponent");
			ImGui::Separator();

			ImGui::Text("Light Component");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			bool removeComponent{ false };
			if (ImGui::Button("X", buttonSize))
			{
				removeComponent = true;
			}

			ImGui::Separator();
			ImGui::Text("Light Type");
			ImGui::SameLine();
			int currentType = static_cast<int>(light.Type);
			if (ImGui::Combo("##Light Type", &currentType, LightComponent::LightTypeNames, IM_ARRAYSIZE(LightComponent::LightTypeNames)))
			{
				light.Type = static_cast<LightType>(currentType);
			}

			ImGui::Text("Light Color");
			ImGui::SameLine();
			ImGui::ColorEdit3("##Color", glm::value_ptr(light.Color));

			/*if (light.Type == LightType::Directional)
			{
				DrawVec3Control("Direction", light.Direction, 0.5f);

			}*/
			
			ImGuiPanel::SliderParams attenuationParams;
			attenuationParams.Speed = 0.001f;
			attenuationParams.Format = "%.4f";
			if (light.Type == LightType::Point)
			{
				ImGuiPanel::DrawVec3Control("Attenuations", light.Attenuation, attenuationParams);

			}

			if (light.Type == LightType::Spot)
			{
				//DrawVec3Control("Direction", light.Direction, 0.5f);
				ImGuiPanel::DrawVec3Control("Attenuations", light.Attenuation, attenuationParams);

				ImGuiPanel::SliderParams params;
				params.Min = 1.0f;
				params.Max = 180.0f;
				params.ResetValue = 12.5f;
				params.Speed = 0.01f;
				ImGuiPanel::DrawFloatControl("Inner Radius", light.InnerRadius, params);

				ImGuiPanel::SliderParams paramsOuter;
				paramsOuter.Min = 1.0f;
				paramsOuter.Max = 180.0f;
				paramsOuter.ResetValue = 15.0f;
				paramsOuter.Speed = 0.01f;
				ImGuiPanel::DrawFloatControl("Outer Radius", light.OuterRadius, paramsOuter);
			}

			if (removeComponent)
			{
				selected.RemoveComponent<LightComponent>();
			}
			ImGui::PopID();

		}
	}

	void DetailsViewPanel::DrawOrthographicCamProps(Camera& camera)
	{
		float labelWidth = (ImGui::GetFontSize() * 10.0f);
		ImGui::SeparatorText("Zoom");

		ImGui::DragFloat("##Value", &camera.m_ZoomLevel, 0.01f, 0.001f);
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			camera.m_ZoomLevel = 1;
		}

		ImGui::SeparatorText("Aspect Ratio");
		ImGui::Text("Manually Set Aspect Ratio");
		ImGui::SameLine();
		glm::vec2 viewportSize = EngineContext::GetEngine()->GetViewportSize();

		static glm::vec2 size = viewportSize;
		ImGui::Checkbox("##lockAspectRatio", &camera.m_LockAspectRatio);

		if (!camera.m_LockAspectRatio)
			ImGui::BeginDisabled();

		ImGuiPanel::SliderParams viewportParams;
		viewportParams.Speed = 0.01f;
		viewportParams.Min = 0.01f;
		viewportParams.ResetValue = 1.0f;
		if (ImGuiPanel::DrawVec2Control("Aspect Ratio", size, viewportParams, labelWidth))
		{
			camera.m_ManualRatio = size.x / size.y;
		}

		if (!camera.m_LockAspectRatio)
			ImGui::EndDisabled();
	}

	void DetailsViewPanel::DrawPerspectiveCamProps(Camera& camera)
	{
		float labelWidth = (ImGui::GetFontSize() * 10.0f);
		std::vector<std::string> labels{ "FoV", "Near Plane", "Far Plane" };
		float fov = camera.m_Fov;
		float nearPlane = camera.m_Near;
		float farPlane = camera.m_Far;
		std::vector<float*> values{ &fov, &nearPlane, &farPlane };
		std::vector<float> resetValues{ 1.0f, 0.1f, 100.0f };
		if (ImGui::BeginTable("##CameraProperties", 2))
		{
			ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, labelWidth);
			ImGui::TableSetupColumn("Values");

			for (int i = 0; i < labels.size(); i++)
			{
				ImGui::PushID(i);
				ImGui::TableNextRow();
				// the label
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(labels[i].c_str());

				// the values
				ImGui::TableSetColumnIndex(1);
				if (ImGui::DragFloat("##Value", values[i], 0.01f))
				{
					switch (i)
					{
						case 0:
						{
							camera.SetFov(fov);
							break;
						}
						case 1:
						{
							camera.SetNearFar(nearPlane, farPlane);
							break;
						}
						case 2:
						{
							camera.SetNearFar(nearPlane, farPlane);
							break;
						}
						default:
							break;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Reset"))
				{
					*values[i] = resetValues[i];
				}
				ImGui::PopID();

			}

			ImGui::EndTable();
		}
	}

	
}