#include "EndMenu.h"
#include "EngineContext.h"
#include "../ExampleGame.h"
#include "../StateMachine/GameStates.h"

namespace Pixie
{
    void EndMenu::Init(const std::string& nextLevelPath)
    {
        m_Game = std::dynamic_pointer_cast<ExampleGame>(EngineContext::GetGame());

        if (!m_Game)
            return;

        m_CurrentLevel = m_Game->GetCurrentLevel();

        if (nextLevelPath == "")
        {
            m_PlayerDied = true;
            m_IsEndOfGame = false;
        }
        else if (nextLevelPath == "END")
        {
            m_PlayerDied = false;
            m_IsEndOfGame = true;
            m_TempLevelPathStorage = "";
        }
        else
        {
            m_PlayerDied = false;
            m_IsEndOfGame = false;
            m_TempLevelPathStorage = nextLevelPath;
        }
    }

    bool EndMenu::Draw()
    {
        std::string windowTitle = m_PlayerDied ? "...You Died..." : "   Congrats   ";
        ImGui::OpenPopup("End Screen");

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
        if (ImGui::BeginPopupModal("End Screen", NULL, ImGuiWindowFlags_NoDocking))
        {
            ImGui::PushFont(NULL, 62.0f);
            ImGui::Text(windowTitle.c_str());
            ImGui::PopFont();
            ImGui::Separator();

            if (m_Game)
                DrawScore();

            ImGui::InvisibleButton("##Spacer1", ImVec2(120.0f, 25.0f));
            ImGui::Separator();
            ImGui::InvisibleButton("##Spacer2", ImVec2(120.0f, 25.0f));
            float buttonOffset1 = 120.0f + ImGui::GetStyle().ItemSpacing.x;
            //buttonOffset1 *= 0.5f;

            float center = ImGui::GetContentRegionAvail().x * 0.5f;

            ImGui::SetCursorPosX(center - buttonOffset1);
            if (ImGui::Button("Try Again", ImVec2(120.0f, 50.0f)))
            {
                OnRestartButtonPressed();
                ImGui::CloseCurrentPopup();
            }

            if (!m_PlayerDied && m_TempLevelPathStorage != "")
            {
                ImGui::SameLine();
                if (ImGui::Button("Next Level", ImVec2(120.0f, 50.0f))) 
                { 
                    OnContinueButtonPressed(); 
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetCursorPosX(center - 60.0f);
            }
            else
            {
                ImGui::SameLine();
            }

            ImGui::SetItemDefaultFocus();

            if (ImGui::Button("Exit", ImVec2(120.0f, 50.0f)))
            {
                OnQuitButtonPressed();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        return true;
    }

    int EndMenu::DrawReturnsInt()
    {
        if (Draw())
            return 1;
        else
            return 0;
    }

    void EndMenu::DrawScore()
    {
        PlayerData scores = m_Game->GetCurrentPlayerData();

        // have enter gamestate set up level indexes
        //int levelIndex = m_Game->GetCurrentLevel();
        std::string levelLabel = "Level " + std::to_string(m_CurrentLevel);

        ImGui::InvisibleButton("##Spacer3", ImVec2(120.0f, 10.0f));
        ImGui::PushFont(NULL, 32.0f);
        ImGuiPanel::CenteredText(levelLabel);
        ImGui::PopFont();

        ImGui::InvisibleButton("##Spacer4", ImVec2(120.0f, 25.0f));

        ImGui::SetCursorPosX(125.0f );

        if (ImGui::BeginTable("Scores", 2))
        {
            ImGui::TableSetupColumn("Labels");
            ImGui::TableSetupColumn("Values");

            // ROW 1 --------------------------------------
            ImGui::TableNextRow();
            ImGui::PushFont(NULL, 28.0f);
            // the label
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Total Score");

            // the values
            ImGui::TableSetColumnIndex(1);
            ImGui::Text(std::to_string(scores.Score).c_str());
            ImGui::PopFont();

            // ROW 2-------------------------------------------
            ImGui::TableNextRow();
            ImGui::PushFont(NULL, 18.0f);
            // the label
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("  Points Collected");

            // the values
            ImGui::TableSetColumnIndex(1);
            ImGui::Text(std::to_string(scores.PointsCollected).c_str());

            // ROW 3-------------------------------------------
            ImGui::TableNextRow();
            // the label
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("  Enemies Destroyed");

            // the values
            ImGui::TableSetColumnIndex(1);
            ImGui::Text(std::to_string(scores.EnemiesDestroyed).c_str());
            ImGui::PopFont();

            // TODO add damage taken to player data

            ImGui::EndTable();
        }

    }

    void EndMenu::OnRestartButtonPressed()
    {
        EngineContext* engine = Pixie::EngineContext::GetEngine();

        engine->RequestSceneChange(m_Game->GetScenePaths()[m_CurrentLevel]);

        Pixie::GameStateChangeRequestEvent event{ Pixie::PlayingState::Type(), "End level Menu, Restart Button" };
        engine->OnEvent(event);
    }
    void EndMenu::OnContinueButtonPressed()
    {
        EngineContext* engine = Pixie::EngineContext::GetEngine();
        
        engine->RequestSceneChange(m_TempLevelPathStorage);
        
        // comented out the index method since that's not hooked up to the end level triggers yet
        // TODO part of the refactor to use game's map of levels and set next level via look up indices
        //engine->RequestSceneChange(m_Game->GetScenePaths()[m_NextLevelIndex]);

        Pixie::GameStateChangeRequestEvent event{ Pixie::PlayingState::Type(), "End level Menu, Restart Button" };
        engine->OnEvent(event);
    }
    void EndMenu::OnQuitButtonPressed()
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