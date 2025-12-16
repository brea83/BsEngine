#include "BsPrecompileHeader.h"
#include "SceneHierarchyPanel.h"
#include <Pixie.h>
#include "ImGui/ImGuiPanel.h"

//#include "Scene/Scene.h"
//#include "Scene/GameObject.h"
//#include "Scene/Components/Component.h"
//#include "Graphics/Primitives/TransformComponent.h"

namespace Pixie
{
	void SceneHierarchyPanel::Draw(Scene* currentScene, GameObject& selected)
	{
		ImGui::Begin("HierarchyTree");
		if (currentScene == nullptr)
		{
			ImGui::End();
			return;// entt::null;
		}

		//static Entity selected = Entity();
		entt::registry& registry = currentScene->m_Registry;


		auto view = registry.view<HeirarchyComponent>();
		if (!view)
		{
			ImGui::End();
			return;// selected;
		}

		for (entt::entity entity : view)
		{
			GameObject currentObject{ entity, currentScene };
			auto [family] = view.get(entity);
			if (family.Parent != entt::null) continue;

			DrawNode(selected, currentObject);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			selected = {};
		}

		// right click on blank space
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				currentScene->CreateEmptyGameObject("Empty Object");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		//return selected;
	}

	void SceneHierarchyPanel::DrawNode(GameObject& selected, GameObject& entity)
	{
		std::string& entityName = entity.GetComponent<NameComponent>().Name;
		std::string name = (entityName.empty()) ? "_NameEmpty_" : entityName;

		//is entity a leaf node ie, no children
		HeirarchyComponent& family = entity.GetComponent<HeirarchyComponent>();
		bool bLeafNode = family.Children.empty();

		ImGuiTreeNodeFlags flags = ((selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (bLeafNode)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}
		else
		{
			flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		}

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
			if (!bLeafNode)
			{
				for (auto childHandle : family.Children)
				{
					GameObject child{ childHandle, entity.m_Scene };
					DrawNode(selected, child);
				}
			}

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			entity.m_Scene->RemoveEntity(entity);
			if (selected == entity)
				selected = {};
		}
	}
}

