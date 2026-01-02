#include "BsPrecompileHeader.h"
#include "ConsoleWindow.h"

namespace Pixie
{
    

    bool ConsoleWindow::Draw()
    {
        ImGui::Begin("Console");
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
        ImGui::PushFont(NULL, 20.0f);

        std::shared_lock<std::shared_timed_mutex> lock(m_LogHistory_mutex);
        for (auto const& log : m_LogHistory)
        {
            bool logPassesFilter = true; //TODO: implement filtering of log msgs in console
            if (!logPassesFilter) continue;

            ImGui::BeginGroup();

            if (log.Emphasis)
            {
                //TODO: implement font weights for imgui
            }

            //if (log.Color_range_start > 0)
            //{
            //    auto propertyLength = log.Properties.size();

            //    std::string currentPart = log.Properties.substr(0, log.Color_range_start);
            //    ImGui::TextUnformatted(currentPart.c_str());
            //    ImGui::SameLine();

            //    currentPart = log.Properties.substr(log.Color_range_start, log.Color_range_end - log.Color_range_start);
            //    ImGui::TextColored(log.Color, "%s", currentPart.c_str());
            //    ImGui::SameLine();

            //    currentPart = log.Properties.substr(log.Color_range_end, propertyLength - log.Color_range_end);
            //    ImGui::TextUnformatted(currentPart.c_str());
            //}
            //else
            //{
            //    if (log.Color_range_end == 1)
            //    {
            //        ImGui::TextColored(log.Color, "%s", log.Properties.c_str());
            //    }
            //    else
            //    {
            //        ImGui::TextUnformatted(log.Properties.c_str());
            //    }
            //}

            //if (log.Color_range_end == 1)
            //{
            //    ImGui::SameLine();
            //    //wrap
            //    ImGui::TextColored(log.Color, "%s", log.Message.c_str());
            //}
            //else
            //{
            //    ImGui::SameLine();
            //    //wrap
            //    ImGui::TextUnformatted(log.Message.c_str());
            //}
            ImGui::TextColored(log.Color, "%s", log.Message.c_str());
            ImGui::EndGroup();
        }

        if (!m_scrollLock && m_scrollToBottom)
        {
            ImGui::SetScrollHereY(1.0F);
        }
        m_scrollToBottom = false;

        ImGui::PopFont();
        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::End();
        return true;
    }

    void ConsoleWindow::RecieveLog(LogData& log)
    {
        std::unique_lock<std::shared_timed_mutex> lock(m_LogHistory_mutex);
        m_LogHistory.push_back(std::move(log));
        m_scrollToBottom = true;
    }

}