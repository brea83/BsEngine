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
        ImGui::Text("The Game Is Paused!");
        ImGui::Separator();


        if (ImGui::Button("Resume", ImVec2(120, 0))) { OnResumeButtonPressed(); }
        ImGui::SetItemDefaultFocus();

        if (ImGui::Button("Exit", ImVec2(120, 0))) 
        { 
            OnExitButtonPressed();
            ImGui::CloseCurrentPopup(); 
        }
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
