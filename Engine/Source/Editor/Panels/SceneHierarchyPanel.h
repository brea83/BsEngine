#pragma once
#include "Scene.h"

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel( );

	void SetContext( Scene* scene);

	//void SetSelected();

	void OnImGuiRender();

private:
	Scene* _currentScene;
};

