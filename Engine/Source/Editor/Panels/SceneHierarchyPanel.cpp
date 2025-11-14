#include "BsPrecompileHeader.h"
#include "SceneHierarchyPanel.h"

//#include <imgui_internal.h>
//#include "misc/cpp/imgui_stdlib.cpp"
#include "Graphics/Primitives/Renderable.h"
#include "Graphics/Primitives/Transform.h"
#include <glm/gtc/type_ptr.hpp>


SceneHierarchyPanel::SceneHierarchyPanel()
{ }

void SceneHierarchyPanel::SetContext( Scene* scene)
{
	_currentScene = scene;
}

void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Hierarchy");

	//left panel
	static int selected = 0;
	Renderable* selectedObject{ nullptr };
	{
		//ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

		for ( int i = 0; i < _currentScene->_objectsToRender.size(); i++)//Renderable* renderObject : _currentScene->_objectsToRender)
		{
			Renderable* renderObject = _currentScene->_objectsToRender[i];
			static const std::string emptyName = "_NameEmpty_";


			if (renderObject->Name.empty())
			{
				if (ImGui::Selectable(emptyName.c_str(), selected == i, ImGuiSelectableFlags_SelectOnNav))
				{
					selected = i;
				}
			}
			else
			{
				if (ImGui::Selectable(renderObject->Name.c_str(), selected == i, ImGuiSelectableFlags_SelectOnNav))
				{
					selected = i;
				}
			}
		}
		//ImGui::EndChild();
	}
	ImGui::End();
	//ImGui::SameLine();
	// right panel
	
	ImGui::Begin("Details View");
	if (_currentScene->_objectsToRender.size() > selected && selected >= 0)
	{
		selectedObject = _currentScene->_objectsToRender[selected];
		//ImGui::Text("%s", selectedObject->Name.c_str());
		ImGui::Separator();
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), selectedObject->Name.c_str());
			if (ImGui::InputText("Name", buffer, sizeof(buffer)))
			{
					
				selectedObject->Name = std::string(buffer);
			}
				
			ImGui::SeparatorText("Transform");
				
			Transform* transform = selectedObject->GetTransform();
			//glm::vec3 position = transform->GetPosition();
			glm::vec3 rotation = transform->GetRotationEuler();
			glm::vec3 scale = transform->GetScale();
			if (DrawVec3Control("Position", transform->_position))
			{
				transform->_positionDirty = true;
			}

			//translate rotation from radians to degrees
			glm::vec3 eulerDegrees = transform->GetRotationEuler();
			if (DrawVec3Control("Rotation", eulerDegrees ))
			{
				transform->SetRotationEuler(eulerDegrees);
			}
			if (DrawVec3Control("Scale", transform->_scale, 1.0f))
			{
				transform->_scaleDirty = true;
			}
				
		}
			
	}
	ImGui::End();
	
}

bool SceneHierarchyPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
{
	bool bValueChanged = false;

	if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable))
	{
		float fontSize = ImGui::GetFontSize();
		ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, fontSize * columnWidth);
		ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();
		// the label
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(label.c_str());

		// the values
		ImGui::TableSetColumnIndex(1);
		ImGui::PushItemWidth(fontSize * 4.0f);
		
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
		if(ImGui::Button("Y"))
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
		if(ImGui::Button("Z"))
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
