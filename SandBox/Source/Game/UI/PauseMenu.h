#pragma once
#include "ImGui/ImGuiPanel.h"
#include <memory>

namespace Pixie
{
	class Scene;
}

class PauseMenu : public Pixie::ImGuiPanel
{
public:
	PauseMenu() = default;
	PauseMenu(std::shared_ptr<Pixie::Scene> currentScene)
		: m_Scene(currentScene) { }
	~PauseMenu() { }

	void SetCurrentScene(std::shared_ptr<Pixie::Scene> scene) { m_Scene = scene; }
	// Inherited via ImGuiPanel
	bool Draw() override;

	int DrawReturnsInt() override;
private:
	std::shared_ptr<Pixie::Scene> m_Scene{ nullptr };

	//void DrawLevelData();
	void OnResumeButtonPressed();
	void OnExitButtonPressed();
	//void QuitToMenu();
	//void QuitToDesktop();

};

