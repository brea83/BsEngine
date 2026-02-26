#include "GameDetailsEditor.h"
#include "ImGui/ImGuiPanel.h"
#include "Game.h"
#include "PlatformUtils.h"

namespace Pixie
{
	void GameDetailsEditor::Draw(std::shared_ptr<Game> game, bool* showCloseButton)
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if(!ImGui::Begin("Game Details Editor", showCloseButton))
		{
			ImGui::End();// end of Game Details Editor window
			return;
		}
		
		game->DrawEditor();
		ImGui::End(); // end of Game Details Editor window
	}
}