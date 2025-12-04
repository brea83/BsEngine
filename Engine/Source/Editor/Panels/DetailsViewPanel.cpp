#include "BsPrecompileHeader.h"
#include "DetailsViewPanel.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Graphics/Primitives/Transform.h"
#include "Scene/Components/Component.h"
#include "Scene/Components/MeshComponent.h"
#include "Scene/Components/CameraComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "Resources/AssetLoader.h"

//DetailsViewPanel::DetailsViewPanel(Scene* scene, std::size_t selectedObjectIndex)
//	:_currentScene(scene), _selected(selectedObjectIndex)
//{}

bool DetailsViewPanel::Draw(Scene* _currentScene, entt::entity selected)
{
	//if (_selected < 0 || _currentScene->m_GameObjects.size() <= _selected)
	//{
	//	return false;
	//}
	//
	//ImGui::Begin("Details View");
	//{
	//	GameObject* selectedObject = _currentScene->m_GameObjects[_selected];
	//	
	//	static bool bIsEditing = false;
	//	static std::string editingValue1;
	//	DrawStringProperty("Name", selectedObject->Name, editingValue1, bIsEditing);


	//	if (ImGui::BeginPopupContextItem("AddComponentPopUp"))
	//	{
	//		if (ImGui::Selectable("Mesh Component"))
	//		{
	//			selectedObject->AddComponent<MeshComponent>();
	//		}

	//		if (ImGui::Selectable("CameraComponent"))
	//		{
	//			//selectedObject->AddComponent<CameraComponent>();
	//		}
	//		ImGui::EndPopup();
	//	}
	//	if(ImGui::Button("AddComponent"))
	//	{
	//		// To Do make pop up to select component type. 
	//		// for now we only have one component tho so test add and remove with that
	//		ImGui::OpenPopup("AddComponentPopUp");

	//	}

	//	ImGui::SeparatorText("Transform");

	//	std::shared_ptr<Transform> transform = selectedObject->GetTransform();
	//	//glm::vec3 position = transform->GetPosition();
	//	glm::vec3 rotation = transform->GetRotationEuler();
	//	glm::vec3 scale = transform->GetScale();
	//	if (DrawVec3Control("Position", transform->m_Position))
	//	{
	//		transform->m_PositionDirty = true;
	//	}

	//	//translate rotation from radians to degrees
	//	glm::vec3 eulerDegrees = transform->GetRotationEuler();
	//	if (DrawVec3Control("Rotation", eulerDegrees))
	//	{
	//		transform->SetRotationEuler(eulerDegrees);
	//	}
	//	if (DrawVec3Control("Scale", transform->m_Scale, 1.0f))
	//	{
	//		transform->m_ScaleDirty = true;
	//	}

	//	

	//	ImGui::SeparatorText("Componenets");
	//	DrawComponents(selectedObject/*selectedObject->GetAllComponents()*/);
	//	
	//}
	//ImGui::End();
	return true;
}


bool DetailsViewPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
{
	bool bValueChanged = false;

	if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable))
	{
		float fontSize = ImGui::GetFontSize();
		ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, fontSize * columnWidth);
		ImGui::TableSetupColumn("Values"/*, ImGuiTableColumnFlags_WidthFixed,*/ );
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
			ImGui::Text( value.c_str());
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
		if(ImGui::Button(buttonText.c_str()))
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

void DetailsViewPanel::DrawComponents(entt::entity selected)
{

	//if (selectedObject->HasCompoenent<MeshComponent>())
	//{
	//	ImGui::PushID("MeshComponent");
	//	ImGui::Separator();
	//	std::shared_ptr<MeshComponent> component = selectedObject->GetComponent<MeshComponent>();
	//	char buffer[256];
	//	memset(buffer, 0, sizeof(buffer));
	//	strcpy_s(buffer, sizeof(buffer), component->Name().c_str());
	//	ImGui::Text(buffer);
	//	ImGui::SameLine();
	//	
	//	float buttonWidth = ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.f);
	//	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - buttonWidth);

	//	if (ImGui::Button("X"))
	//	{
	//		selectedObject->RemoveComponent<MeshComponent>();
	//	}

	//	ImGui::Separator();

	//	//ImGui::Text(component->GetFilePath().c_str());
	//	static bool bIsEditing2 = false;
	//	static std::string  editingValue2;
	//	std::string previousValue2 = component->m_FilePath;
	//	if (DrawStringProperty("Mesh File", component->m_FilePath, editingValue2, bIsEditing2))
	//	{

	//		if (!component->Reload())
	//		{
	//			std::cout << "Error loading mesh file, reverting to old mesh path" << std::endl;
	//			component->m_FilePath = previousValue2;
	//		}
	//	}

	//	static bool bIsEditing = false;
	//	static std::string  editingValue1;
	//	std::string previousValue = component->m_TexturePath;
	//	if (DrawStringProperty("Texture File", component->m_TexturePath, editingValue1, bIsEditing))
	//	{
	//		std::shared_ptr<Texture> newTexture = AssetLoader::LoadTexture(component->m_TexturePath);
	//		if (newTexture == nullptr)
	//		{
	//			std::cout << "Error loading Texture file, reverting to old Texture path" << std::endl;
	//			component->m_TexturePath = previousValue;
	//		}
	//		else
	//		{
	//			if (component->m_Texture != nullptr)
	//			{
	//				component->m_Texture = newTexture;
	//			}
	//		}
	//	}
	//	ImGui::PopID();
	//}

	//if (selectedObject->HasCompoenent<CameraComponent>())
	//{
		//ImGui::PushID("CameraComponent");
		//ImGui::Separator();
		//std::shared_ptr<CameraComponent> component = selectedObject->GetComponent<CameraComponent>();
		//char buffer[256];
		//memset(buffer, 0, sizeof(buffer));
		//strcpy_s(buffer, sizeof(buffer), component->Name().c_str());
		//ImGui::Text(buffer);
		//ImGui::SameLine();

		//float buttonWidth = ImGui::CalcTextSize("X").x + (ImGui::GetStyle().FramePadding.x * 2.f);
		//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - buttonWidth);

		//if (ImGui::Button("X"))
		//{
		//	selectedObject->RemoveComponent<CameraComponent>();
		//}

		//ImGui::Separator();

		//float labelWidth = ( ImGui::GetFontSize() * 10.0f);
		//std::vector<std::string> labels{ "FoV", "Near Plane", "Far Plane" };
		//std::vector<float*> values{ &component->m_Fov, &component->m_Near, &component->m_Far };
		//if(ImGui::BeginTable("##CameraProperties", 2))
		//{
		//	ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, labelWidth);
		//	ImGui::TableSetupColumn("Values");

		//	for (int i = 0; i < labels.size(); i++)
		//	{
		//		ImGui::PushID(i);
		//		ImGui::TableNextRow();
		//		// the label
		//		ImGui::TableSetColumnIndex(0);
		//		ImGui::Text(labels[i].c_str());

		//		// the values
		//		ImGui::TableSetColumnIndex(1);
		//		ImGui::DragFloat("##Value", values[i]);
		//		ImGui::PopID();

		//	}

		//	ImGui::EndTable();
		//}
		//ImGui::PopID();
	//}
}