#pragma once
#include "Scene.h"
#include <glm/glm.hpp>

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel( );

	void SetContext( Scene* scene);

	//void SetSelected();

	void OnImGuiRender();

private:
	Scene* _currentScene;

	///Returns true if one of the vector values is changed
	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 5.0f);
};

