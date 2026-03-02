#include "HUD.h"

namespace Pixie
{
	bool HUD::Draw()
	{
        static int location = 0;
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking 
            | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings 
            | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowBgAlpha(0.55f); // Transparent background

        //if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
        //if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
        //if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
        //if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
        //if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
        //if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
        if (location >= 0)
        {
            const float PAD = 50.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
           
            //ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            //ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoMove;
        }

        if (ImGui::Begin("Example: Simple overlay", NULL, window_flags))
        {
            ImGui::Text("Controls");
            ImGui::Separator();
            ImGui::Text("W: Up");
            ImGui::Text("A: Left");
            ImGui::Text("S: Down");
            ImGui::Text("D: Right");

            ImGui::Separator();
            ImGui::Text("SpaceBar: Boost");
            ImGui::Text("Left Shift: Break");
           
        }
        ImGui::End();
		return true;
	}
	int HUD::DrawReturnsInt()
	{
		if (Draw())
			return 1;
		else
			return 0;
	}
}