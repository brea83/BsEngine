#pragma once
#include "ImGuiPanel.h"
#include "Log.h"
#include <shared_mutex>

namespace Pixie
{
    struct LogData
    {
        LogData() = default;

        LogLevel Level{LogLevel::Off};
        //std::string Properties;
        //std::string Source;
        std::string Message{ "" };
        //std::size_t Color_range_start{ 0 };
        //std::size_t Color_range_end{ 0 };
        ImVec4 Color{ 0.9F, 0.7F, 0.0F, 1.0F };
        bool Emphasis{ false };
    };

    class ConsoleWindow : public ImGuiPanel
    {
    public:
        ConsoleWindow() {}
        virtual ~ConsoleWindow() {}

        bool IsTimeShown() const { return m_ShowTime; }
        bool IsLevelShown() const { return m_ShowLevel; }
        bool IsLoggerShown() const { return m_ShowLogger; }

        // Inherited via ImGuiPanel
        bool Draw() override;
        // do not use, not implemented
        int DrawReturnsInt() override { return 0; };

        void RecieveLog(LogData& log);
    protected:

        std::vector<LogData> m_LogHistory;
        mutable std::shared_timed_mutex m_LogHistory_mutex;

        ImGuiTextFilter m_DisplayFilter;

        bool m_scrollToBottom{ false };
        bool m_wrap{ false };
        bool m_scrollLock{ false };

        bool m_ShowTime{ true };
        bool m_ShowLevel{ true };
        bool m_ShowLogger{ true };

    };

}