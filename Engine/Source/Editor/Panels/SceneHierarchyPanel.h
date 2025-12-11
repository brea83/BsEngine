#pragma once
#include "ImGuiPanel.h"

namespace Pixie
{
	class Scene;
}

class SceneHierarchyPanel// : public ImGuiPanel
{
public:
	static void Draw(Pixie::Scene* currentScene, GameObject& selected);
	static void DrawNode(GameObject& selected, GameObject& entity);
private:

	
};

