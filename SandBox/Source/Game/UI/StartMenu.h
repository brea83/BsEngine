#pragma once
#include "ImGui/ImGuiPanel.h"
#include <memory>

namespace Pixie
{
	class ExampleGame;

	class StartMenu : public ImGuiPanel
	{
	public:
		StartMenu() = default;

		void Init();
		bool Draw() override;
		int DrawReturnsInt() override;

	private:
		std::shared_ptr<ExampleGame> m_Game{ nullptr };

		void DrawLevelSelect();
		void RequestLevelStart(int levelIndex);
		void OnQuitButtonPressed();
	};

}