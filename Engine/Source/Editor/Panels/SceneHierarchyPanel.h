#pragma once
#include "ImGuiPanel.h"
#include "Scene/GameObject.h"

namespace Pixie
{
	class Scene;

	class SceneHierarchyPanel
	{
	public:
		static void Draw(Scene* currentScene, GameObject& selected);
		static void DrawNode(GameObject& selected, GameObject& entity);
	private:
	};
}
