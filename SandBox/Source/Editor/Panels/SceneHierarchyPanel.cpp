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
		entt::registry& registry = currentScene->GetRegistry();


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
			if (family.Parent != 0) continue;

			DrawNode(selected, currentObject);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			selected = {};
		}

		// right click on blank space
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				currentScene->CreateEmptyGameObject("Empty Object");
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Add Camera"))
			{
				GameObject newCamera = currentScene->CreateEmptyGameObject("Camera");
				TransformComponent& transform = newCamera.GetTransform();
				transform.SetPosition(glm::vec3(0.0f, 0.0f, -15.0f));
				transform.SetRotationEuler(glm::vec3(0.0f, 180.0f, 0.0f));
				newCamera.AddComponent<CameraComponent>();
				newCamera.AddComponent<CameraController>();
			}

			ImGui::Separator();
			if (ImGui::BeginMenu("Add Mesh Object"))
			{
				if (ImGui::MenuItem("Cube"))
				{
					currentScene->CreateCube();
				}
				ImGui::Separator();

				if (ImGui::MenuItem("Sphere"))
				{
					currentScene->CreateSphere();
				}
				ImGui::Separator();

				if (ImGui::MenuItem("Plane XZ"))
				{
					currentScene->CreatePlane(glm::vec3(-90.0f, 0.0f, 0.0f));
				}

				if (ImGui::MenuItem("Plane XY"))
				{
					currentScene->CreatePlane();
				}

				if (ImGui::MenuItem("Plane YZ"))
				{
					currentScene->CreatePlane(glm::vec3( 0.0f, -90.0f, 0.0f));
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();
			if (ImGui::BeginMenu("Add Light"))
			{
				
					if (ImGui::MenuItem("Directional Light"))
					{
						currentScene->TryCreateDirectionalLight();
					}

					if (ImGui::MenuItem("Point Light"))
					{
						GameObject newLight = currentScene->CreateEmptyGameObject("Point Light");
						LightComponent& light = newLight.AddComponent<LightComponent>();
						light.Type = LightType::Point;
					}

					if (ImGui::MenuItem("Spot Light"))
					{
						GameObject newLight = currentScene->CreateEmptyGameObject("Spot Light");
						TransformComponent& transform = newLight.GetTransform();
						transform.SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
						transform.SetRotationEuler(glm::vec3(-90.0f, 180.0f, 0.0f));

						LightComponent& light = newLight.AddComponent<LightComponent>();
						light.Type = LightType::Spot;
					}

					ImGui::EndMenu();
				
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
		//HeirarchyComponent& family = entity.GetComponent<HeirarchyComponent>();
		std::vector<GameObject> children = entity.GetChildren();
		bool bLeafNode = children.empty();

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
				for (auto child : children)
				{
					DrawNode(selected, child);
				}
			}

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			entity.m_Scene->RemoveGameObject(entity);
			if (selected == entity)
				selected = {};
		}
	}
}

