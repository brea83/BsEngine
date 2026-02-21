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
		static std::string editingName = game->m_Title;

		ImGuiPanel::DrawStringProperty("Title", game->m_Title, editingName);

		if (ImGui::Button("Save"))
		{
			if (game->m_SettingsPath.empty())
			{
				std::filesystem::path filePath = FileDialogs::SaveFile("Pixie Game Settings (*.pixieIni)\0*.pixieIni\0");

				if (!filePath.empty())
				{
					game->SaveSettings(filePath);
				}
			}
			else
			{
				game->SaveSettings(game->m_SettingsPath);
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Load"))
		{

			std::filesystem::path filePath = FileDialogs::OpenFile("Pixie Game Settings (*.pixieIni)\0*.pixieIni\0");

			if (!filePath.empty())
			{
				game->LoadSettings(filePath);
			}
		}

		ImGui::SeparatorText("Scenes");
		std::vector<std::filesystem::path>& scenes = game->GetScenePathsEditable();
		
		if (ImGui::Button("Add Scene"))
		{
			std::filesystem::path filePath = FileDialogs::OpenFile("Scene Files (*.pixie)\0*.pixie\0");

			if (!filePath.empty())
			{
				game->AddScenePath(filePath);
			}
		}

		std::vector<int> scenesToRemove;
		int sceneListLength = scenes.size();
		for (int i = 0; i < sceneListLength; i++)
		{
			ImGui::PushID(i);
			std::string label = "Scene " + std::to_string(i);
			std::string pathString = scenes[i].string();

			if (ImGuiPanel::FileProperty(label, pathString, "Scene Files (*.pixie)\0*.pixie\0"))
			{
				scenes[i] = pathString;
			}

			ImGui::SameLine();
			ImGui::PushID("RemoveScene");
			if (ImGui::Button("X"))
			{
				scenesToRemove.push_back(i);
			}
			ImGui::PopID();
			ImGui::PopID();
		}

		for (auto index : scenesToRemove)
		{
			game->RemoveScene(index);
		}


		ImGui::End(); // end of Game Details Editor window
	}
}