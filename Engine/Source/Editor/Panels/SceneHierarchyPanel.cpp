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
				currentScene->RemoveEntity(entity);
			}
			ImGui::PopID();
		}
		
		ImGui::EndTable();
	}
	
	ImGui::End();
	
	return selected;
	
}

entt::entity SceneHierarchyPanel::DrawTree(Scene* currentScene)
{
	ImGui::Begin("HierarchyTree");
	if (currentScene == nullptr)
	{
		ImGui::End();
		return entt::null;
	}

	static Entity selected = Entity();
	entt::registry& registry = currentScene->m_Registry;


	auto view = registry.view<Transform>();
	for (entt::entity entity : view)
	{
		GameObject currentObject{ entity, currentScene };
		HeirarchyComponent& family = currentObject.GetComponent<HeirarchyComponent>();
		if (family.Parent != entt::null) continue;

		DrawNode(selected, currentObject);
			
	}

	ImGui::End();

	return selected;
}

void SceneHierarchyPanel::DrawNode(Entity& selected, Entity& entity)
{
	std::string& name = entity.GetComponent<NameComponent>().Name;

	ImGuiTreeNodeFlags flags = ((selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
	if (ImGui::IsItemClicked())
	{
		selected = entity;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		HeirarchyComponent& family = entity.GetComponent<HeirarchyComponent>();
		if ( family.Children.size() > 0)
		{
			for (auto childHandle : family.Children)
			{
				Entity child{ childHandle, entity.m_Scene };
				DrawNode(selected, child);
			}
		}
		/*bool opened = ImGui::TreeNodeEx((void*)9817239, flags, "test");
		if (opened)
			ImGui::TreePop();*/
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		entity.m_Scene->RemoveEntity(entity);
		if (selected == entity)
			selected = {};
	}

}
