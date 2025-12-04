#pragma once
#include "ImGuiPanel.h"
#include "Scene/Entity.h"
class Scene; 

class SceneHierarchyPanel// : public ImGuiPanel
{
public:
	//SceneHierarchyPanel( );

	//void SetContext( Scene* scene);

	//void SetSelected();

	static entt::entity Draw(Scene* currentScene = nullptr);
	//std::size_t GetSelectedIndex() const { return _selected; }
private:
	//Scene* currentScene{ nullptr };
	//std::size_t _selected{ 0 };

	
};

