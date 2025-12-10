#include "BsPrecompileHeader.h"
#include "SceneHierarchyPanel.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
using namespace Pixie;

int SceneHierarchyPanel::Draw(Scene* _currentScene)
{

	

	ImGui::Begin("Hierarchy");
	
	static int _selected = 0;
	if (ImGui::BeginTable("##HierarchyTable", 2))
	{

		if (_currentScene->m_GameObjects.size() <= 0)
		{
			ImGui::EndTable();
			ImGui::End();
			return -1;
		}

		float fontSize = ImGui::GetFontSize();
		ImGui::TableSetupColumn("Objects", ImGuiTableColumnFlags_WidthFixed, fontSize * 20.0f);
		ImGui::TableSetupColumn("DeleteButton", ImGuiTableColumnFlags_WidthStretch);

		for ( int i = 0; i < _currentScene->m_GameObjects.size(); i++)//Renderable* renderObject : _currentScene->m_MeshComponents)
		{
			ImGui::TableNextRow();
			// Column 1
			ImGui::TableSetColumnIndex(0);

			GameObject* gameObject = _currentScene->m_GameObjects[i];
			static const std::string emptyName = "_NameEmpty_";


			if (gameObject->Name.empty())
			{
				if (ImGui::Selectable(emptyName.c_str(), _selected == i, ImGuiSelectableFlags_SelectOnNav))
				{
					_selected = i;
				}
			}
			else
			{
				if (ImGui::Selectable(gameObject->Name.c_str(), _selected == i, ImGuiSelectableFlags_SelectOnNav))
				{
					_selected = i;
				}
			}

			// the values
			ImGui::TableSetColumnIndex(1);

			ImGui::PushID(i);
			if (ImGui::Button("X"))
			{
				_currentScene->RemoveGameObject(gameObject);
			}
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
	
	ImGui::End();
	
	return _selected;
	
}
