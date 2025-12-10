#pragma once
#include "ImGuiPanel.h"
#include "Scene/Entity.h"
class Scene; 

class SceneHierarchyPanel// : public ImGuiPanel
{
public:
	static void Draw(Scene* currentScene, GameObject& selected);
	static void DrawNode(GameObject& selected, GameObject& entity);
private:

	
};

