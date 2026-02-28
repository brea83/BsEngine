#pragma once
#include "ImGui/ImGuiPanel.h"
#include <memory>

namespace Pixie
{
	class ExampleGame;

	class EndMenu : public ImGuiPanel
	{
	public:
		EndMenu() = default;

		// Inherited via ImGuiPanel
		void Init(int levelIndex);//const std::string& nextLevelPath);
		bool Draw() override;
		int DrawReturnsInt() override;

		void SetEndGame(bool value) { m_IsEndOfGame = value; }
		void SetPlayerDied(bool value) { m_PlayerDied = value; }
		void SetNextLevel(int index) { m_NextLevelIndex = index; }

	private:
		std::shared_ptr<ExampleGame> m_Game{nullptr};
		bool m_IsEndOfGame{ false };
		bool m_PlayerDied{ false };

		// -1 for invalid
		int m_NextLevelIndex{ -1 };
		std::string m_TempLevelPathStorage{ "" }; // TODO REFACTOR TO JUST USE THE GAME LIST INDEXES

		int m_CurrentLevel{ -1 };

		void DrawScore();

		void OnRestartButtonPressed();
		void OnContinueButtonPressed();
		void OnQuitButtonPressed();
	};
}