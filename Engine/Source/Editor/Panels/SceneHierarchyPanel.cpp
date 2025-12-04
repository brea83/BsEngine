#include "BsPrecompileHeader.h"
#include "SceneHierarchyPanel.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Scene/Components/Component.h"
#include "Graphics/Primitives/Transform.h"

//#include <imgui_internal.h>
//#include "misc/cpp/imgui_stdlib.cpp"
//#include "Graphics/Primitives/Renderable.h"

//#include <glm/gtc/type_ptr.hpp>

//
//SceneHierarchyPanel::SceneHierarchyPanel()
//{ }
//
//void SceneHierarchyPanel::SetContext( Scene* scene)
//{
//	currentScene = scene;
//}

entt::entity SceneHierarchyPanel::Draw(Scene* currentScene)
{

	ImGui::Begin("Hierarchy");
	if (currentScene == nullptr)
	{
		ImGui::End();
		return entt::null;
	}

	static entt::entity selected = entt::null;
	entt::registry& registry = currentScene->m_Registry;

	if (ImGui::BeginTable("##HierarchyTable", 2))
	{

		float fontSize = ImGui::GetFontSize();
		ImGui::TableSetupColumn("Objects", ImGuiTableColumnFlags_WidthFixed, fontSize * 20.0f);
		ImGui::TableSetupColumn("DeleteButton", ImGuiTableColumnFlags_WidthStretch);

		auto view = registry.view<Transform>();
		for (entt::entity entity : view)
		{
			ImGui::TableNextRow();
			// Column 1
			ImGui::TableSetColumnIndex(0);

			static const std::string emptyName = "_NameEmpty_";

			NameComponent* nameComponent = registry.try_get<NameComponent>(entity);
			if (nameComponent == nullptr || nameComponent->Name.empty())
			{
				if (ImGui::Selectable(emptyName.c_str(), selected == entity, ImGuiSelectableFlags_SelectOnNav))
				{
					selected = entity;
				}
			}
			else
			{
				if (ImGui::Selectable(nameComponent->Name.c_str(), selected == entity, ImGuiSelectableFlags_SelectOnNav))
				{
					selected = entity;
				}
			}

			// the values
			ImGui::TableSetColumnIndex(1);

			ImGui::PushID(int(entity));
			if (ImGui::Button("X"))
			{
				currentScene->RevoveObjectByEntity(entity);
			}
			ImGui::PopID();
		}
		
		ImGui::EndTable();
	}
	
	ImGui::End();
	
	return selected;
	
}
