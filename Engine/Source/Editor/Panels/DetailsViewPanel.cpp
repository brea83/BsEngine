#include "BsPrecompileHeader.h"
#include "DetailsViewPanel.h"
#include "Scene.h"
#include "GameObject.h"
#include "Graphics/Primitives/Transform.h"
#include "Component.h"
#include "Graphics/Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "AssetLoader.h"

//DetailsViewPanel::DetailsViewPanel(Scene* scene, std::size_t selectedObjectIndex)
//	:_currentScene(scene), _selected(selectedObjectIndex)
//{}

bool DetailsViewPanel::Draw(Scene* _currentScene, int _selected)
{
	if (_selected < 0 || _currentScene->_gameObjects.size() <= _selected)
	{
		return false;
	}
	
	GameObject* selectedObject{ nullptr };
	ImGui::Begin("Details View");
	{
		selectedObject = _currentScene->_gameObjects[_selected];
		//ImGui::Text("%s", selectedObject->Name.c_str());
		ImGui::Separator();
		{
			/*char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), selectedObject->Name.c_str());
			if (ImGui::InputText("Name", buffer, sizeof(buffer)))
			{

				selectedObject->Name = std::string(buffer);
			}*/
			static bool bIsEditing = false;
			static std::string editingValue1;
			//DrawStringProperty("Name", selectedObject->Name, editingValue1, bIsEditing);

			ImGui::SeparatorText("Transform");

			std::shared_ptr<Transform> transform = selectedObject->GetTransform();
			//glm::vec3 position = transform->GetPosition();
			glm::vec3 rotation = transform->GetRotationEuler();
			glm::vec3 scale = transform->GetScale();
			if (DrawVec3Control("Position", transform->_position))
			{
				transform->_positionDirty = true;
			}

			//translate rotation from radians to degrees
			glm::vec3 eulerDegrees = transform->GetRotationEuler();
			if (DrawVec3Control("Rotation", eulerDegrees))
			{
				transform->SetRotationEuler(eulerDegrees);
			}
			if (DrawVec3Control("Scale", transform->_scale, 1.0f))
			{
				transform->_scaleDirty = true;
			}

		}

		ImGui::SeparatorText("Componenets");
		DrawComponents(selectedObject/*selectedObject->GetAllComponents()*/);
		
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

	if (ImGui::BeginTable(label.c_str(), 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter))
	{
		float fontSize = ImGui::GetFontSize();
		ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, fontSize * 25.0f);
		ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthFixed, ImGui::GetContentRegionAvail().x * 0.5);
		ImGui::TableSetupColumn("EditButton", ImGuiTableColumnFlags_WidthStretch/*, -FLT_MIN*/);

		ImGui::TableNextRow();
		// the label
		ImGui::TableSetColumnIndex(0);
		ImGui::PushItemWidth(fontSize * 3);
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
		ImGui::TableSetColumnIndex(2);
		ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x);

		std::string buttonText = bIsEditing ? "Done" : "Edit";
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
		ImGui::PopItemWidth();
		ImGui::EndTable();
	}

	return bValueSubmitted;
}

void DetailsViewPanel::DrawComponents(GameObject* selectedObject)
{

	if (selectedObject->HasCompoenent<Model>())
	{
		std::shared_ptr<Model> component = selectedObject->GetComponent<Model>();
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), component->Name.c_str());
		ImGui::SeparatorText(buffer);

		//ImGui::Text(component->GetFilePath().c_str());
		static bool bIsEditing2 = false;
		static std::string  editingValue2;
		std::string previousValue2 = component->_filePath;
		if (DrawStringProperty("Mesh File", component->_filePath, editingValue2, bIsEditing2))
		{

			if (!component->Reload())
			{
				std::cout << "Error loading mesh file, reverting to old mesh path" << std::endl;
				component->_filePath = previousValue2;
			}
		}

		static bool bIsEditing = false;
		static std::string  editingValue1;
		std::string previousValue = component->_texturePath;
		if (DrawStringProperty("Texture File", component->_texturePath, editingValue1, bIsEditing))
		{
			std::shared_ptr<Texture> newTexture = AssetLoader::LoadTexture(component->_texturePath);
			if (newTexture == nullptr)
			{
				std::cout << "Error loading Texture file, reverting to old Texture path" << std::endl;
				component->_texturePath = previousValue;
			}
			else
			{
				if (component->_meshes[0]->Textures.size() > 0)
				{
					component->_meshes[0]->Textures[0] = newTexture;
				}
				else
				{
					component->_meshes[0]->Textures.push_back(newTexture);
				}
			}
		}
	}
}