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
	template <typename T>
	void DrawComponent(Pixie::GameObject& selected, bool removeable)
	{
		T* componentPointer = selected.TryGetComponent<T>();
		IComponentStruct* downCastStruct = dynamic_cast<IComponentStruct*>(componentPointer);
		IComponentClass* downCastClass = dynamic_cast<IComponentClass*>(componentPointer);
		if ( downCastStruct == nullptr && downCastClass == nullptr)
			return;

		T& component = selected.GetComponent<T>();

		std::string name = component.GetName();
		ImGui::PushID(name.c_str());

		if (removeable)
		{
			bool bDontDeleteHeader = true;
			ImGuiChildFlags flags = ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_ResizeY;
			if (ImGui::CollapsingHeader(name.c_str(), &bDontDeleteHeader, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(20, 0, 20, 100));
				ImGui::SetCursorPosX(ImGui::GetTreeNodeToLabelSpacing());
				ImGui::BeginChild((name + "child").c_str(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), flags, ImGuiWindowFlags_None);
				ImGui::PopStyleColor();

				component.Draw(selected);

				ImGui::EndChild();
			}

			if (!bDontDeleteHeader)
			{
				selected.RemoveComponent<T>();
			}
		}
		else
		{
			ImGuiChildFlags flags = ImGuiChildFlags_ResizeY;
			if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(20, 0, 20, 100));
				ImGui::SetCursorPosX(ImGui::GetTreeNodeToLabelSpacing());
				ImGui::BeginChild((name + "child").c_str(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), flags, ImGuiWindowFlags_None);
				ImGui::PopStyleColor();

				component.Draw(selected);

				ImGui::EndChild();
			}
		}
		
		ImGui::PopID();
	}

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
			DrawComponent<TransformComponent>(selected, false);
		}

		if (selected.HasCompoenent<TagComponent>())
		{
			DrawComponent<TagComponent>(selected, true);
		}

		if (selected.HasCompoenent<SplineComponent>())
		{
			DrawComponent<SplineComponent>(selected, true);
		}

		if (selected.HasCompoenent<PlayerInputComponent>())
		{
			DrawComponent<PlayerInputComponent>(selected, true);
		}

		if (selected.HasCompoenent<MovementComponent>())
		{
			DrawComponent<MovementComponent>(selected, true);
		}

		if (selected.HasCompoenent<MovementConstraintsComponent>())
		{
			DrawComponent<MovementConstraintsComponent>(selected, true);
		}

		if (selected.HasCompoenent<FollowComponent>())
		{
			DrawComponent<FollowComponent>(selected, true);
		}

		if (selected.HasCompoenent<OrbitComponent>())
		{
			DrawComponent<OrbitComponent>(selected, true);
		}

		if (selected.HasCompoenent<NativeScriptComponent>())
		{
			DrawComponent<NativeScriptComponent>(selected, true);
		}

		if (selected.HasCompoenent<MeshComponent>())
		{
			DrawComponent<MeshComponent>(selected, true);
		}

		if (selected.HasCompoenent<CircleRendererComponent>())
		{
			DrawComponent<CircleRendererComponent>(selected, true);
			/*ImGui::PushID("CircleRenderer");
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

			ImGui::PopID();*/
		}

		if (selected.HasCompoenent<CollisionComponent>())
		{
			DrawComponent<CollisionComponent>(selected, true);
			//ImGui::PushID("CollisionComponent");
			//ImGui::Separator();
			//CollisionComponent& component = selected.GetComponent<CollisionComponent>();
			//ImGui::Text("Collision Component");
			//ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25.0f);

			//ImVec2 buttonSize{ ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			//ImGui::CalcTextSize("X").y + (ImGui::GetStyle().FramePadding.y * 2.0f) };

			//bool removeComponent{ false };
			//if (ImGui::Button("X", buttonSize))
			//{
			//	removeComponent = true;
			//}

			//ImGui::Separator();

			//ImGui::Text("Collider Type");
			//ImGui::SameLine();
			//int currentType = static_cast<int>(component.Type);
			//if (ImGui::Combo("##ColliderType", &currentType, CollisionComponent::TypeNames, IM_ARRAYSIZE(CollisionComponent::TypeNames)))
			//{
			//	ColliderType newType = static_cast<ColliderType>(currentType);

			//	if (newType != component.Type)
			//	{
			//		component.Type = newType;
			//		scene->GetRegistry().patch<CollisionComponent>(selected.GetEnttHandle());
			//	}
			//}
			//ImGui::Checkbox("Active", &component.BIsActive);


			//// specific colliders

			//if (selected.HasCompoenent<SphereCollider>())
			//{
			//	SphereCollider& collider = selected.GetComponent<SphereCollider>();
			//	ImGuiPanel::SliderParams params;
			//	params.ResetValue = 0.5f;
			//	params.Speed = 0.001f;
			//	params.Min = 0.0f;

			//	ImGuiPanel::DrawFloatControl("Radius", collider.Radius, params);
			//}

			//if (selected.HasCompoenent<CubeCollider>())
			//{
			//	CubeCollider& collider = selected.GetComponent<CubeCollider>();
			//	ImGuiPanel::SliderParams params;
			//	params.ResetValue = 0.5f;
			//	params.Speed = 0.001f;
			//	params.Min = 0.0f;

			//	ImGuiPanel::DrawVec3Control("Extents", collider.Extents, params);
			//	ImGui::SetItemTooltip("Extents are the HALF width/height/depth values");
			//}

			//if (removeComponent)
			//{
			//	selected.RemoveComponent<CollisionComponent>();
			//}

			//ImGui::PopID();
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
			DrawComponent<LightComponent>(selected, true);
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