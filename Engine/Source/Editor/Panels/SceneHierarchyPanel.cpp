#include "BsPrecompileHeader.h"
#include "SceneHierarchyPanel.h"
#include <imgui.h>
#include<backends/imgui_impl_glfw.h>
#include<backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
//#include "misc/cpp/imgui_stdlib.cpp"
#include "Graphics/Primitives/Renderable.h"
#include "Graphics/Primitives/Transform.h"

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
		ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

		for ( int i = 0; i < _currentScene->_objectsToRender.size(); i++)//Renderable* renderObject : _currentScene->_objectsToRender)
		{
			Renderable* renderObject = _currentScene->_objectsToRender[i];
			static const std::string emptyName = "_NameEmpty_";
			if (renderObject->Name.empty())
			{
				if (ImGui::Selectable(emptyName.c_str(), selected == i, ImGuiSelectableFlags_SelectOnNav))
				{
					selected = i;
					//selectedObject = _currentScene->_objectsToRender[i];
				}
			}
			else
			{
				if (ImGui::Selectable(renderObject->Name.c_str(), selected == i, ImGuiSelectableFlags_SelectOnNav))
				{
					selected = i;
					//selectedObject = _currentScene->_objectsToRender[i];
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();
	// right panel
	{
		ImGui::BeginChild("Details View", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
		if (_currentScene->_objectsToRender.size() > selected && selected >= 0)
		{
			selectedObject = _currentScene->_objectsToRender[selected];
			//ImGui::Text("%s", selectedObject->Name.c_str());
			ImGui::Separator();
			// property names on left
			{
				//ImGui::BeginChild("property names", ImVec2(50, 0), ImGuiChildFlags_ResizeX);
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, selectedObject->Name.c_str());
				if (ImGui::InputText("Name", buffer, sizeof(buffer)))
				{
					
					selectedObject->Name = std::string(buffer);
				}
				
				ImGui::SeparatorText("Transform");
				//ImGui::Text("Transform");
				glm::vec3 position = selectedObject->GetTransform()->GetPosition();
				float v[3]{ position.x, position.y, position.z };

				ImGui::InputFloat3("Pos", v, "%.3f", 0);
				/*ImGui::EndChild();*/
			}
			
		}
		


		ImGui::EndChild();
	}
	ImGui::End();
}