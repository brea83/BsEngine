#include "BsPrecompileHeader.h"
#include "StartUpWindow.h"
#include "Game.h"
#include "ImGui/ImGuiPanel.h"
#include "PlatformUtils.h"
#include "EngineContext.h"

namespace Pixie
{
    bool StartUpWindow::DrawGameStartupWindow(std::shared_ptr<Game> game)
    {
        static bool gameLoaded = false;

        ImGui::OpenPopup("Select Game");
        
        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_None, ImVec2(0.5f, 0.5f));

        ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
        if (ImGui::BeginPopupModal("Select Game", NULL, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::PushFont(NULL, 42.0f);
            ImGuiPanel::CenteredText("Select Game File");
            ImGui::PopFont();

            ImGui::InvisibleButton("##Spacer1", ImVec2(120.0f, 25.0f));

            ImGui::PushFont(NULL, 20.0f);
            float buttonOffset = 120.0f + ImGui::GetStyle().ItemSpacing.x;
            float center = ImGui::GetContentRegionAvail().x * 0.5f;

            ImGui::SetCursorPosX(center - buttonOffset);
            if (ImGui::Button("New", ImVec2(120 + ImGui::GetStyle().ItemSpacing.x, 40)))
            {
                if (OnNewGameButtonPressed(game))
                {
                    // returns true if game saved
                    gameLoaded = true;
                }
            }
            ImGui::SetItemDefaultFocus();

            ImGui::SameLine();

            if (ImGui::Button("Load", ImVec2(120, 40)))
            {
                gameLoaded = OnLoadGameButtonPressed(game);
            }
            ImGui::PopFont();

            ImGui::Separator();

            game->DrawPreviewData();

            ImGui::Separator();

            if (gameLoaded)
            {
                ImGui::SetCursorPosX(center - 124.0f);
                if (ImGui::Button("Confirm Game", ImVec2(240.0f, 50.0f)))
                {
                    ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                    return true;
                }
            }

            ImGui::InvisibleButton("##Spacer5", ImVec2(120.0f, 25.0f));

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.25f, 0.25f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 0.2f, 0.2f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.0f, 0.05f, 1.0f });

            ImGui::SetCursorPosX(center - 124.0f);
            if (ImGui::Button("Exit", ImVec2(240.0f, 40.0f)))
            {
                OnQuitButtonPressed();
                ImGui::CloseCurrentPopup();
            }

            ImGui::PopStyleColor(3);
            ImGui::EndPopup();
        }
        return false;
    }
    
    bool StartUpWindow::OnNewGameButtonPressed(std::shared_ptr<Game> game)
    {
        ImGui::OpenPopup("New Game");
        bool newGameSaved = false;
        if (ImGui::BeginPopupModal("New Game", NULL, ImGuiWindowFlags_NoDocking))
        {
            static std::string editingValue = "Title";
            static std::string title = game->GetTitle();
            if(ImGuiPanel::DrawStringProperty("New Title", title, editingValue))
            {
                game->SetTitle(title);
                std::filesystem::path filePath = FileDialogs::SaveFile("Pixie Game Settings (*.pixieIni)\0*.pixieIni\0");

                if (!filePath.empty())
                {
                    game->SaveSettings(filePath);
                    newGameSaved = true;
                }
                ImGui::CloseCurrentPopup();
            }

            if(ImGui::Button("Use Default Title"))
            {
                std::filesystem::path filePath = FileDialogs::SaveFile("Pixie Game Settings (*.pixieIni)\0*.pixieIni\0");

                if (!filePath.empty())
                {
                    game->SaveSettings(filePath);
                    newGameSaved = true;
                }
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::BeginItemTooltip())
            {
                ImGui::Text("Default Title Can be changed from game settings window later.");
            }

            ImGui::EndPopup();
        }

        return newGameSaved;
    }
        
    bool StartUpWindow::OnLoadGameButtonPressed(std::shared_ptr<Game> game)
    {
        std::filesystem::path filePath = FileDialogs::OpenFile("Pixie Game Settings (*.pixieIni)\0*.pixieIni\0");

        if (!filePath.empty())
        {
            game->LoadSettings(filePath);
            return true;
        }
        return false;
    }

    void StartUpWindow::OnQuitButtonPressed()
    {
        WindowClosedEvent event;
        EngineContext::GetEngine()->OnEvent(event);

    }
}