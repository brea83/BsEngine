#pragma once
#include "ImGuiPanel.h"

class Scene; 

class SceneHierarchyPanel// : public ImGuiPanel
{
public:
	//SceneHierarchyPanel( );

	//void SetContext( Scene* scene);

	//void SetSelected();

	static int Draw(Scene* _currentScene = nullptr);
	//std::size_t GetSelectedIndex() const { return _selected; }

private:
	//Scene* _currentScene{ nullptr };
	//std::size_t _selected{ 0 };

	
};

