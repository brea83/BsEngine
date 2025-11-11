#pragma once
#include "Scene.h"
#include <glm/glm.hpp>
#include "ImGuiPanel.h"

class SceneHierarchyPanel : public ImGuiPanel
{
public:
	SceneHierarchyPanel( );

	void SetContext( Scene* scene);

	//void SetSelected();

	void OnImGuiRender() override;

private:
	Scene* _currentScene{ nullptr };

	///Returns true if one of the vector values is changed
	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 5.0f);
};

