#include "BsPrecompileHeader.h"
#include "SceneHierarchyPanel.h"
#include <imgui.h>
#include<backends/imgui_impl_glfw.h>
#include<backends/imgui_impl_opengl3.h>
#include "Graphics/Primitives/Renderable.h"

SceneHierarchyPanel::SceneHierarchyPanel()
{ }

void SceneHierarchyPanel::SetContext( Scene* scene)
{
	_currentScene = scene;
}

void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Hierarchy");
	for (Renderable* renderObject : _currentScene->_objectsToRender)
	{
		ImGui::Text("%s", renderObject->Name.c_str());
	}

	ImGui::End();
}