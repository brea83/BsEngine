#include "StartMenu.h"
#include "EngineContext.h"
#include "../ExampleGame.h"
#include "../StateMachine/GameStates.h"

namespace Pixie
{
	void StartMenu::Init()
	{
		m_Game = std::dynamic_pointer_cast<ExampleGame>(EngineContext::GetGame());

		if (!m_Game)
			return;
	}
	bool StartMenu::Draw()
	{
        if (m_Game == nullptr)
        {
            ImGui::Text("Uh Oh No Game Loaded");
			return false;
        }

        
        ImGui::OpenPopup("Title Screen");

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
        if (ImGui::BeginPopupModal("Title Screen", NULL, ImGuiWindowFlags_NoDocking))
        {
            ImGui::PushFont(NULL, 62.0f);
            ImGuiPanel::CenteredText(m_Game->GetTitle());
            ImGui::PopFont();

            ImGui::Separator();
			ImGui::InvisibleButton("##Spacer1", ImVec2(120.0f, 25.0f));

			DrawLevelSelect();

            ImGui::InvisibleButton("##Spacer2", ImVec2(120.0f, 25.0f));
            ImGui::Separator();
            ImGui::InvisibleButton("##Spacer3", ImVec2(120.0f, 25.0f));
            
            float center = ImGui::GetContentRegionAvail().x * 0.5f;
            ImGui::SetCursorPosX(center - 124.0f);
            if (ImGui::Button("Exit", ImVec2(240.0f, 50.0f)))
            {
                OnQuitButtonPressed();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        return true;
	}

	int StartMenu::DrawReturnsInt()
	{
		if (Draw())
			return 1;
		else
			return 0;
	}

	void StartMenu::DrawLevelSelect()
	{
		std::string labelBase = "Level ";
		for (auto& const pair : m_Game->GetAllLevelData())
		{
			int levelIndex = pair.first;
			if (levelIndex == 0)
				continue; // level 0 reserved for title menu

			if (ImGui::Button((labelBase + std::to_string(levelIndex)).c_str()))
			{
				RequestLevelStart(levelIndex);
			}

			ImGui::PushID(levelIndex);
			ImGui::InvisibleButton(("##Spacer4"), ImVec2(120.0f, 10.0f));
			ImGui::PopID();
		}
	}

	void StartMenu::RequestLevelStart(int levelIndex)
	{
		if (!m_Game)
			return;
		m_Game->RequestStateChange(PlayingState::Type());
		m_Game->RequestLevelChange(levelIndex);
	}

	void StartMenu::OnQuitButtonPressed()
	{
		if (Pixie::EngineContext::GetEngine()->IsEditorEnabled())
		{
			Pixie::GameStateChangeRequestEvent event{ Pixie::EditState::Type(), "End level Menu, Exit Button" };
			Pixie::EngineContext::GetEngine()->OnEvent(event);
		}
		else
		{
			Pixie::WindowClosedEvent event;
			Pixie::EngineContext::GetEngine()->OnEvent(event);
		}
	}
}