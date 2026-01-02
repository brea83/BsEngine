#pragma once
#include "ImGui/ConsoleWindow.h"
#include "spdlog/sinks/base_sink.h"

#define COLOR_WARN 0.9F, 0.7F, 0.0F, 1.0F 
#define COLOR_ERROR 1.0F, 0.0F, 0.0F, 1.0F 

namespace spdlog
{
	template<typename Mutex>
	class imgui_sink : public sinks::base_sink <Mutex>
	{
    public:
        imgui_sink(std::shared_ptr<Pixie::ConsoleWindow> consoleWindow = nullptr)
            :  m_ConsoleWindow(consoleWindow)
        {}

        //void SetConsoleWindow(std::shared_ptr<ConsoleWindow> console) { m_ConsoleWindow = console; }

    protected:
        std::shared_ptr<Pixie::ConsoleWindow> m_ConsoleWindow{ nullptr };


        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            // log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
            // msg.payload (before v1.3.0: msg.raw) contains pre formatted log
            
            if (m_ConsoleWindow == nullptr) return; // early out since there's nothing to send the data to

            Pixie::LogData data;
            
            using LEVEL = level::level_enum;
            switch (msg.level)
            {
            case LEVEL::trace:
                data.Level = Pixie::LogLevel::Trace;
                data.Color = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
                break;
            case LEVEL::debug:
                data.Level = Pixie::LogLevel::Debug;
                data.Color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
                break;
            case LEVEL::info:
                data.Level = Pixie::LogLevel::Info;
                data.Color = ImGui::GetStyleColorVec4(ImGuiCol_NavHighlight);
                break;
            case LEVEL::warn:
                data.Level = Pixie::LogLevel::Warning;
                data.Color = ImVec4{ COLOR_WARN };
                break;
            case LEVEL::err:
                data.Level = Pixie::LogLevel::Error;
                data.Color = ImVec4{ COLOR_ERROR };
                break;
            case LEVEL::critical:
                data.Level = Pixie::LogLevel::Critical;
                data.Color = ImVec4{ COLOR_ERROR };
                break;
            default:
                data.Level = Pixie::LogLevel::Off;
                data.Color = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
                break;
            }
            
            // If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

            data.Message = fmt::to_string(formatted);

            
            m_ConsoleWindow->RecieveLog(data);
        }

        void flush_() override
        {
            std::cout << std::flush;
        }

	};

    #include "spdlog/details/null_mutex.h"
    #include <mutex>
    using imgui_sink_mt = imgui_sink<std::mutex>;
    using imgui_sink_st = imgui_sink<spdlog::details::null_mutex>;
}
