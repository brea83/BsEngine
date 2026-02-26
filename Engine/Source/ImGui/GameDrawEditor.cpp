#include "BsPrecompileHeader.h"
#include "Game.h"
#include "ImGuiPanel.h"
#include "ScriptManager.h"

#include "PlatformUtils.h"

namespace Pixie
{
	void Game::DrawEditor()
	{
		static std::string editingName = m_Title;

		ImGuiPanel::DrawStringProperty("Title", m_Title, editingName);

		if (ImGui::Button("Save"))
		{
			if (m_SettingsPath.empty())
			{
				std::filesystem::path filePath = FileDialogs::SaveFile("Pixie Game Settings (*.pixieIni)\0*.pixieIni\0");

				if (!filePath.empty())
				{
					SaveSettings(filePath);
				}
			}
			else
			{
				SaveSettings(m_SettingsPath);
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Load"))
		{

			std::filesystem::path filePath = FileDialogs::OpenFile("Pixie Game Settings (*.pixieIni)\0*.pixieIni\0");

			if (!filePath.empty())
			{
				LoadSettings(filePath);
			}
		}

		ImGui::SeparatorText("Scenes");
		std::vector<std::filesystem::path>& scenes = GetScenePathsEditable();

		if (ImGui::Button("Add Scene"))
		{
			std::filesystem::path filePath = FileDialogs::OpenFile("Scene Files (*.pixie)\0*.pixie\0");

			if (!filePath.empty())
			{
				AddScenePath(filePath);
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
			RemoveScene(index);
		}
	}
}