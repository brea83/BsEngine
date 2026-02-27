#include "BsPrecompileHeader.h"
#include "../../Game/ExampleGame.h"
#include "ImGui/ImGuiPanel.h"
#include "ScriptManager.h"

#include "PlatformUtils.h"

namespace Pixie
{

	void ExampleGame::DrawSceneList()
	{
		if (ImGui::Button("Add Scene"))
		{
			std::filesystem::path filePath = FileDialogs::OpenFile("Scene Files (*.pixie)\0*.pixie\0");

			if (!filePath.empty())
			{
				AddScenePath(filePath);
			}
		}

		std::vector<int> scenesToRemove;
		int sceneListLength = m_ScenePaths.size();
		for (int i = 0; i < sceneListLength; i++)
		{
			ImGui::PushID(i);
			std::string label = "Scene " + std::to_string(i);
			std::string pathString = m_ScenePaths[i].string();
			float availableWidth = ImGui::GetContentRegionAvail().x;
			float buttonWidth = ImGuiPanel::GetTextSizePadded().x;
			if (ImGuiPanel::FileProperty(label, pathString, "Scene Files (*.pixie)\0*.pixie\0", "X", availableWidth - buttonWidth))
			{
				m_ScenePaths[i] = pathString;
			}
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				ImGui::SetDragDropPayload(m_DragDropScene.c_str(), &i, sizeof(int));
				ImGui::Text(("Swap "+ label).c_str());
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_DragDropScene.c_str()))
				{
					IM_ASSERT(payload->DataSize == sizeof(int));
					int droppedIndex= *(const int*)payload->Data;

					std::filesystem::path tmp = m_ScenePaths[i];
					m_ScenePaths[i] = m_ScenePaths[droppedIndex];
					m_ScenePaths[droppedIndex] = tmp;
				}
				ImGui::EndDragDropTarget();
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

	void ExampleGame::DrawLevelList()
	{
		

		std::vector<int> levelToRemove;
		int levelMapSize = m_LevelData.size();
		for (int i = 0; i < levelMapSize; i++)
		{
			if (m_LevelData.find(i) == m_LevelData.end())
				continue;

			ImGui::PushID(i);
			std::string label = "Level " + std::to_string(i);
			int filePathIndex = m_LevelData[i].FilePathIndex;
			std::string pathString = filePathIndex == -1 ? "" : m_ScenePaths[filePathIndex].filename().string();
			
			ImGui::BeginGroup();// drag and drop group

			ImGui::Text(label.c_str());
			ImGui::SameLine();
			ImGui::Text(pathString.c_str());

			ImGui::EndGroup();// drag and drop group
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				ImGui::SetDragDropPayload("LEVEL LIST", &i, sizeof(int));
				ImGui::Text(("Swap " + label).c_str());
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL LIST"))
				{
					IM_ASSERT(payload->DataSize == sizeof(int));
					int droppedIndex = *(const int*)payload->Data;

					Level tmp = m_LevelData[i];
					m_LevelData[i] = m_LevelData[droppedIndex];
					m_LevelData[droppedIndex] = tmp;
				}
				else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_DragDropScene.c_str()))
				{
					IM_ASSERT(payload->DataSize == sizeof(int));
					int droppedIndex = *(const int*)payload->Data;

					m_LevelData[i].FilePathIndex = droppedIndex;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();
			ImGui::PushID("RemoveLevel");
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (ImGuiPanel::GetTextSizePadded().x + ImGui::GetStyle().ItemSpacing.x));
			if (ImGui::Button("X"))
			{
				levelToRemove.push_back(i);
				m_LevelData[i].FilePathIndex = -1;
			}
			ImGui::PopID();
			ImGui::PopID();
		}

		if (!levelToRemove.empty())
		{
			std::unordered_map<int, Level> newLevelList;
			for (int i = 0; i < m_LevelData.size(); i++)
			{
				if (m_LevelData[i].FilePathIndex == -1)
					continue; // skip empty data

				int newLevelNum = newLevelList.size();
				newLevelList[newLevelNum] = Level(m_LevelData[i]);
			}
			m_LevelData.swap(newLevelList);
				
		}
		
	}

	void ExampleGame::DrawEditor()
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

		float availableWidth = ImGui::GetContentRegionAvail().x;
		// scenes list
		ImGui::BeginChild("Scenes", ImVec2(availableWidth * 0.5f, 260), ImGuiChildFlags_None, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("Scenes List");
				ImGui::EndMenuBar();
			}
			DrawSceneList();

		}
		ImGui::EndChild(); // levels list window

		ImGui::SameLine();

		ImGui::BeginChild("Levels", ImVec2(availableWidth * 0.5f, 260), ImGuiChildFlags_None, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("Levels List");
				if (ImGui::BeginMenu("Add Level"))
				{
					for (int i = 0; i < m_ScenePaths.size(); i++)
					{
						std::string name = m_ScenePaths[i].stem().string();
						if (ImGui::MenuItem(name.c_str()))
						{
							Level newLevel;
							newLevel.FilePathIndex = i;
							int newLevelNum = m_LevelData.size();
							m_LevelData[newLevelNum] = newLevel;
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			DrawLevelList();

		}
		ImGui::EndChild(); // scenes list window
	}

	void ExampleGame::DrawPreviewData()
	{
		ImGui::PushFont(NULL, 42.0f);
		ImGuiPanel::CenteredText(m_Title);
		ImGui::PopFont();

		auto windowWidth = ImGui::GetWindowSize().x;

		ImGui::BeginChild("Scene List", ImVec2(windowWidth * 0.3f, 100.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
		if (m_ScenePaths.size() > 0)
		{
			if (ImGui::BeginTable("ScenesList", 1, ImGuiTableFlags_RowBg))
			{
				ImGui::TableSetupColumn("Scenes");
				ImGui::TableHeadersRow();

				for (auto scene : m_ScenePaths)
				{
					std::string name = scene.filename().string();

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(name.c_str());
				}
				ImGui::EndTable();
			}
		}
		ImGui::EndChild(); // end scene list --------------------------

		ImGui::SameLine();

		ImGui::BeginChild("Level List", ImVec2(windowWidth * 0.3f, 100.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
		if (m_LevelData.size() > 0)
		{
			std::string levelBase = "Level ";

			if (ImGui::BeginTable("Levels", 2, ImGuiTableFlags_RowBg ))
			{

				ImGui::TableSetupColumn("Level");
				ImGui::TableSetupColumn("Scene");
				ImGui::TableHeadersRow();

				for (auto& pair : m_LevelData)
				{

					int sceneIndex = pair.second.FilePathIndex;
					std::string scene = m_ScenePaths[sceneIndex].filename().string();

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text((levelBase + std::to_string(pair.first)).c_str());
					ImGui::TableSetColumnIndex(1);
					ImGui::Text(scene.c_str());
				}
				ImGui::EndTable();
			}

		}
		ImGui::EndChild(); // end Level list --------------------------
		
		ImGui::SameLine();
		// start states list --------------------------
		std::vector<std::string> states = m_GameStateMachine.GetLoadedStateTypes();
		if (states.empty())
			return;
		
		ImGui::BeginChild("States List", ImVec2(windowWidth * 0.3f, 100.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);

		if (ImGui::BeginTable("StatesList", 1, ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Game States");
			ImGui::TableHeadersRow();

			for (auto& name : states)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(name.c_str());
			}
			ImGui::EndTable();
		}

		ImGui::EndChild(); // end states list --------------------------
	}
}