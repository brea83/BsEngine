#include "BsPrecompileHeader.h"
#include "DetailsViewPanel.h"
#include "Pixie.h"
#include "ImGui/ImGuiPanel.h"
//#include "Scene/Scene.h"
//#include "Graphics/Primitives/TransformComponent.h"
//#include "Scene/Components/Component.h"
//#include "Scene/Components/MeshComponent.h"
//#include "Scene/Components/CameraComponent.h"
//#include "Graphics/CameraController.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "Resources/AssetLoader.h"
#include <CameraManager.h>

//DetailsViewPanel::DetailsViewPanel(Scene* scene, std::size_t selectedObjectIndex)
//	:_currentScene(scene), _selected(selectedObjectIndex)
//{}
namespace Pixie
{
	bool DetailsViewPanel::Draw(Scene* scene, GameObject& selected)
	{

		ImGui::Begin("Details View");
		{
			if (scene == nullptr)
			{
				ImGui::End();
				return false;
			}
			entt::registry& registry = scene->GetRegistry();
			if (!registry.valid(selected))
			{
				ImGui::End();
				return false;
			}

			static bool bIsEditing = false;
			static std::string editingValue1;
			NameComponent& nameComp = selected.GetComponent<NameComponent>();
			DrawStringProperty("Name", nameComp.Name, editingValue1, bIsEditing);


			if (ImGui::BeginPopupContextItem("AddComponentPopUp"))
			{
				if (ImGui::Selectable("Mesh Component"))
				{
					selected.AddComponent<MeshComponent>();
				}

				if (ImGui::Selectable("Camera Component"))
				{
					selected.AddComponent<CameraComponent>();
					selected.AddComponent<CameraController>();
				}
				if (ImGui::Selectable("Light"))
				{
					selected.AddComponent<LightComponent>();
				}
				ImGui::EndPopup();
			}
			if (ImGui::Button("AddComponent"))
			{
				ImGui::OpenPopup("AddComponentPopUp");
			}


			//	

			ImGui::SeparatorText("Componenets");
			DrawComponents(scene, selected/*selectedObject->GetAllComponents()*/);

		}
		ImGui::End();
		return true;
	}


	bool DetailsViewPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
	{
		bool bValueChanged = false;

		if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable))
		{
			float fontSize = ImGui::GetFontSize();
			ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, fontSize * columnWidth);
			ImGui::TableSetupColumn("Values"/*, ImGuiTableColumnFlags_WidthFixed,*/);
			ImGui::TableNextRow();
			// the label
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(label.c_str());

			// the values
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);// fontSize * 4.0f);

			ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.25f, 0.25f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.0f, 0.05f, 1.0f });
			if (ImGui::Button("X"))
			{
				values.x = resetValue;
				bValueChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##floatX", &values.x, 0.1f))
			{
				bValueChanged = true;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();

			ImGui::SameLine();
			ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.3f, 0.55f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.6f, 0.05f, 1.0f });
			if (ImGui::Button("Y"))
			{
				values.y = resetValue;
				bValueChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##floatY", &values.y, 0.1f))
			{
				bValueChanged = true;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();

			ImGui::SameLine();
			ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.25f, 0.25f, 0.7f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 1.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.0f, 0.6f, 1.0f });
			if (ImGui::Button("Z"))
			{
				values.z = resetValue;
				bValueChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##floatZ", &values.z, 0.1f))
			{
				bValueChanged = true;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();

			ImGui::PopItemWidth();

			ImGui::EndTable();
		}

		return bValueChanged;
	}

	bool DetailsViewPanel::DrawFloatControl(const std::string& label, float& value, SliderParams params, float columnWidth)
	{
		bool bValueChanged = false;

		if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable))
		{
			float fontSize = ImGui::GetFontSize();
			ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, fontSize * columnWidth);
			ImGui::TableSetupColumn("Values"/*, ImGuiTableColumnFlags_WidthFixed,*/);
			ImGui::TableNextRow();
			// the label
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(label.c_str());

			// the values
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);

			ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, 0.0f);
			if (ImGui::DragFloat("##float", &value, params.Speed, params.Min, params.Max, params.Format.c_str(), params.Flags))
			{
				bValueChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				value = params.ResetValue;
				bValueChanged = true;
			}

			ImGui::PopStyleVar();

			
			ImGui::PopItemWidth();

			ImGui::EndTable();
		}

		return bValueChanged;
	}


	bool DetailsViewPanel::DrawStringProperty(const std::string& label, std::string& value, std::string& editingValue, bool& bIsEditing, float columnWidth)
	{
		bool bValueChanged = false;
		//bool bIsEditing = false;
		bool bValueSubmitted = false;

		if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable/* | ImGuiTableFlags_RowBg*/))
		{
			float fontSize = ImGui::GetFontSize();
			ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, fontSize * columnWidth);
			ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthStretch);
			//ImGui::TableSetupColumn("EditButton", ImGuiTableColumnFlags_WidthStretch/*, -FLT_MIN*/);

			ImGui::TableNextRow();
			// the label
			ImGui::TableSetColumnIndex(0);
			ImGui::PushItemWidth(fontSize * columnWidth);
			ImGui::Text(label.c_str());
			ImGui::PopItemWidth();

			// the values
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(fontSize * value.size());
			// do stuff

			if (bIsEditing)
			{
				ImGui::InputTextWithHint("##EditableString", value.c_str(), &editingValue);
			}
			else
			{
				ImGui::Text(value.c_str());
			}
			ImGui::PopItemWidth();

			// the button to turn  the value field into an edit field
			//ImGui::TableSetColumnIndex(2);
			//ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x);
			ImGui::EndTable();

			ImGui::SameLine();
			std::string buttonText = bIsEditing ? "Done" : "Edit";
			float buttonWidth = ImGui::CalcTextSize(buttonText.c_str()).x + (ImGui::GetStyle().FramePadding.x * 2.f);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - buttonWidth);
			ImGui::PushID(label.c_str());
			if (ImGui::Button(buttonText.c_str()))
			{
				bValueChanged = (value != editingValue);
				if (bIsEditing && bValueChanged)
				{
					if (editingValue != "" && editingValue != " ")
					{
						value = editingValue;
						bValueSubmitted = true;
					}
					//else
					//{
					//	//std::cout << "Error: tried to submit empty string to property: " << label << std::endl;
					//}

				}

				bIsEditing = !bIsEditing;
			}
			ImGui::PopID();
			//ImGui::PopItemWidth();
		}

		return bValueSubmitted;
	}

	void DetailsViewPanel::DrawComponents(Scene* scene, GameObject& selected)
	{
		entt::registry& registry = scene->GetRegistry();
		if (selected.HasCompoenent<TransformComponent>())
		{
			ImGui::SeparatorText("Transform");

			TransformComponent& transform = selected.GetTransform();
			//glm::vec3 position = transform->GetPosition();
			//glm::vec3 rotation = transform.GetRotationEuler();
			//glm::vec3 scale = transform.GetScale();
			if (DrawVec3Control("Position", transform.m_Position))
			{
				transform.m_PositionDirty = true;
			}

			//translate rotation from radians to degrees
			glm::vec3 eulerDegrees = transform.GetRotationEuler();
			if (DrawVec3Control("Rotation", eulerDegrees))
			{
				transform.SetRotationEuler(eulerDegrees);
			}
			if (DrawVec3Control("Scale", transform.m_Scale, 1.0f))
			{
				transform.m_ScaleDirty = true;
			}

			ImGui::TextWrapped("There is a known issue with ImGuizmo's Rotation gizmo:");
			ImGui::TextWrapped("If the camera forward and right vectors are paralel to one of the gizmo circle planes those handles will not behave.");
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

			//ImGui::Text(component->GetFilePath().c_str());
			static bool bMeshPathEditing = false;
			static std::string  meshPath;
			std::string previousMeshPath = component.m_FilePath;
			if (DrawStringProperty("Mesh File", component.m_FilePath, meshPath, bMeshPathEditing))
			{
				if (!component.Reload())
				{
					std::cout << "Error loading mesh file, reverting to old mesh path" << std::endl;
					component.m_FilePath = previousMeshPath;
				}
			}

			ImGui::SeparatorText("Material Instance");
			MaterialInstance& material = component.m_MaterialInstance;
			static bool bBaseTextureEditing = false;
			static std::string  baseMapPath;
			std::string previousBasePath = material.BaseMapPath;
			if (DrawStringProperty("Base Texture", material.BaseMapPath, baseMapPath, bBaseTextureEditing))
			{
				std::shared_ptr<Texture> newTexture = AssetLoader::LoadTexture(material.BaseMapPath);
				if (newTexture == nullptr)
				{
					std::cout << "Error loading Texture file, reverting to old Texture path" << std::endl;
					material.BaseMapPath = previousBasePath;
				}
				else
				{
					component.m_MaterialInstance.BaseMap = newTexture;
				}
			}

			static bool bMetallicMapEditing = false;
			static std::string  metallicMapPath;
			std::string previousMetalPath = material.MetallicMapPath;
			if (DrawStringProperty("Metallic Map", material.MetallicMapPath, metallicMapPath, bMetallicMapEditing))
			{
				std::shared_ptr<Texture> newTexture = AssetLoader::LoadTexture(material.MetallicMapPath);
				if (newTexture == nullptr)
				{
					std::cout << "Error loading Texture file, reverting to old Texture path" << std::endl;
					material.MetallicMapPath = previousMetalPath;
				}
				else
				{
					component.m_MaterialInstance.MetallicMap = newTexture;
				}
			}
			ImGui::SetItemTooltip("Expects Metalness and smoothness packed into R and G channels");

			SliderParams smoothnessParams;
			smoothnessParams.Min = 0.0f;
			smoothnessParams.Max = 1.0f;
			smoothnessParams.Speed = 0.001f;
			smoothnessParams.ResetValue = 0.3f;
			DrawFloatControl("Smoothness", material.Smoothness, smoothnessParams);

			SliderParams specParams;
			specParams.Min = 1.0f;
			specParams.Max = 200.0f;
			specParams.Speed = 0.1f;
			specParams.ResetValue = 32.0f;
			DrawFloatControl("Specular Power", material.SpecularPower, specParams);

			if (removeComponent)
			{
				selected.RemoveComponent<MeshComponent>();
			}
			ImGui::PopID();
		}

		if (selected.HasCompoenent<CameraComponent>())
		{
			CameraManager& camManager = scene->GetCameraManager();
			ImGui::PushID("CameraComponent");
			ImGui::Separator();
			CameraComponent& component = selected.GetComponent<CameraComponent>();
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), component.Name.c_str());
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

			if (camManager.GetActiveCameraObject() != selected)
			{
				if (ImGui::Button("Make Active")) // once cam manager is set up turn this into a Possess button
				{
					camManager.SetActiveCamera(selected);
				}
			}
			else
			{
				ImGui::BeginDisabled();
				ImGui::Button("Is Active"); // once cam manager is set up turn this into an Unpossess button
				ImGui::EndDisabled();
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



			float labelWidth = (ImGui::GetFontSize() * 10.0f);
			std::vector<std::string> labels{ "FoV", "Near Plane", "Far Plane" };
			std::vector<float*> values{ &component.Cam.m_Fov, &component.Cam.m_Near, &component.Cam.m_Far };
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
					ImGui::DragFloat("##Value", values[i]);
					ImGui::PopID();

				}

				ImGui::EndTable();
			}

			if (removeComponent)
			{
				selected.RemoveComponent<CameraComponent>();
			}
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

			if (light.Type == LightType::Directional)
			{
				DrawVec3Control("Direction", light.Direction, 0.5f);

			}
			
			if (light.Type == LightType::Point)
			{
				DrawVec3Control("Attenuations", light.Attenuation);

			}

			if (light.Type == LightType::Spot)
			{
				DrawVec3Control("Direction", light.Direction, 0.5f);
				DrawVec3Control("Attenuations", light.Attenuation);

				SliderParams params;
				params.Min = 1.0f;
				params.Max = 180.0f;
				params.ResetValue = 12.5f;
				params.Speed = 0.01f;
				DrawFloatControl("Inner Radius", light.InnerRadius, params);

				SliderParams paramsOuter;
				paramsOuter.Min = 1.0f;
				paramsOuter.Max = 180.0f;
				paramsOuter.ResetValue = 15.0f;
				paramsOuter.Speed = 0.01f;
				DrawFloatControl("Inner Radius", light.OuterRadius, paramsOuter);
			}

			if (removeComponent)
			{
				selected.RemoveComponent<LightComponent>();
			}
			ImGui::PopID();

		}
	}
}