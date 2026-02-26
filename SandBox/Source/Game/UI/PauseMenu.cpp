#include "PauseMenu.h"
#include "Scene/Scene.h"
#include "EngineContext.h"
#include "../StateMachine/GameStates.h"
#include "Events/ApplicationEvent.h"

void PauseMenu::OnResumeButtonPressed()
{
    Pixie::GameStateChangeRequestEvent event{ Pixie::PlayingState::Type(), "Pause Menu, Resume Button"};
    Pixie::EngineContext::GetEngine()->OnEvent(event);
}

void PauseMenu::OnExitButtonPressed()
{
    if (Pixie::EngineContext::GetEngine()->IsEditorEnabled())
    {
        Pixie::GameStateChangeRequestEvent event{ Pixie::EditState::Type(), "Pause Menu, Exit Button" };
        Pixie::EngineContext::GetEngine()->OnEvent(event);
    }
    else
    {
        Pixie::WindowClosedEvent event;
        Pixie::EngineContext::GetEngine()->OnEvent(event);
    }
}

bool PauseMenu::Draw()
{
    if(m_Scene == nullptr)
        return false;

    ImGui::OpenPopup("Paused!");

    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Paused!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::PushFont(NULL, 62.0f);
        ImGui::Text("The Game Is Paused!");
        ImGui::PopFont();

        ImGui::Separator();
        ImGui::InvisibleButton("##Spacer1", ImVec2(120.0f, 25.0f));

        ImGui::PushFont(NULL, 20.0f);
        float buttonOffset = 120.0f + ImGui::GetStyle().ItemSpacing.x;
        float center = ImGui::GetContentRegionAvail().x * 0.5f;

        ImGui::SetCursorPosX(center - buttonOffset);
        if (ImGui::Button("Resume", ImVec2(240 + ImGui::GetStyle().ItemSpacing.x, 50)))
        { 
            OnResumeButtonPressed();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();


        ImGui::SetCursorPosX(center - buttonOffset);
        if (ImGui::Button("Title Menu", ImVec2(120, 40)))
        {
            Pixie::GameStateChangeRequestEvent event{ Pixie::TitleState::Type(), "Pause Menu, Title Menu Button" };
            Pixie::EngineContext::GetEngine()->OnEvent(event);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.25f, 0.25f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.0f, 0.05f, 1.0f });
        if (ImGui::Button("Exit", ImVec2(120, 40))) 
        { 
            OnExitButtonPressed();
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::PopStyleColor(3);
        ImGui::PopFont(); // end larger font started before buttons
        ImGui::EndPopup();
    }

    return true;
}

int PauseMenu::DrawReturnsInt()
{
    if (Draw())
        return 1;
    else
        return 0;
}
