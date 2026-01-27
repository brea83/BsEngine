#pragma once
#include <functional>
#include <spdlog/spdlog.h>
#include <iostream>

namespace Pixie
{
	enum class LogLevel
	{
		Trace,
		Info,
		Debug,
		Warning,
		Error,
		Critical,
		Off
	};


	class Logger
	{
	public:

		static void Init();
		
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetSandboxLogger() { return s_SandboxLogger; }

		// usage 
		//Pixie::Logger::Core(LOG_TRACE, "trace with formatting {}", 12);
		//Pixie::Logger::Core(LOG_INFO, "info with formatting int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
		//Pixie::Logger::Core(LOG_DEBUG, "debug with formatting, Positional args are {1} {0}..", "too", "supported");
		//Pixie::Logger::Core(LOG_WARNING, "warning with formatting Support for floats {:03.2f}", 1.23456);
		//Pixie::Logger::Core(LOG_ERROR, "error with formatting {:04d}", 12);
		//Pixie::Logger::Core(LOG_CRITICAL, "CRITICAL MSG with formatting {:02d}", 12);
		template <typename... Args>
		inline static void Core(LogLevel logLevel, spdlog::format_string_t<Args...> format, Args &&... args) 
		{
			if (!s_CoreLogger)
			{	
				std::cerr << "ERROR:: Logger::Core() called before Logger::Init()" << std::endl;
				return;
			}
			switch (logLevel)
			{
			case Pixie::LogLevel::Trace:
				s_CoreLogger->trace(format, std::forward<Args>(args)...);
				break;
			case Pixie::LogLevel::Info:
				s_CoreLogger->info(format, std::forward<Args>(args)...);
				break;
			case Pixie::LogLevel::Debug:
				s_CoreLogger->debug(format, std::forward<Args>(args)...);
				break;
			case Pixie::LogLevel::Warning:
				s_CoreLogger->warn(format, std::forward<Args>(args)...);
				break;
			case Pixie::LogLevel::Error:
				s_CoreLogger->error(format, std::forward<Args>(args)...);
				break;
			case Pixie::LogLevel::Critical:
				s_CoreLogger->critical(format, std::forward<Args>(args)...);
				break;
			default:
				s_CoreLogger->trace(format, std::forward<Args>(args)...);
				break;
			}
		}

		// usage 
		//Pixie::Logger::Game(LOG_TRACE, "trace with formatting {}", 12);
		//Pixie::Logger::Game(LOG_INFO, "info with formatting int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
		//Pixie::Logger::Game(LOG_DEBUG, "debug with formatting, Positional args are {1} {0}..", "too", "supported");
		//Pixie::Logger::Game(LOG_WARNING, "warning with formatting Support for floats {:03.2f}", 1.23456);
		//Pixie::Logger::Game(LOG_ERROR, "error with formatting {:04d}", 12);
		//Pixie::Logger::Game(LOG_CRITICAL, "CRITICAL MSG with formatting {:02d}", 12);
		template <typename... Args>
		inline static void Game(LogLevel logLevel, spdlog::format_string_t<Args...> format, Args &&... args)
		{
			if (!s_SandboxLogger)
			{
				std::cerr << "ERROR:: Logger::Debug() called before Logger::Init()" << std::endl;
				return;
			}

			switch (logLevel)
			{
			case Pixie::LogLevel::Trace:
				s_SandboxLogger->trace(format, std::forward<Args>(args)...);
				break;
			case Pixie::LogLevel::Info:
				s_SandboxLogger->info(format, std::forward<Args>(args)...);
				break;
			case Pixie::LogLevel::Debug:
				s_SandboxLogger->debug(format, std::forward<Args>(args)...);
				break;
			case Pixie::LogLevel::Warning:
				s_SandboxLogger->warn(format, std::forward<Args>(args)...);
				break;
			case Pixie::LogLevel::Error:
				s_SandboxLogger->error(format, std::forward<Args>(args)...);
				break;
			case Pixie::LogLevel::Critical:
				s_SandboxLogger->critical(format, std::forward<Args>(args)...);
				break;
			default:
				s_SandboxLogger->trace(format, std::forward<Args>(args)...);
				break;
			}
		}

		static void SetEngineCallback(std::function<void(const spdlog::details::log_msg& msg)> function) { s_EngineCallback = function; }
		static void SetSandboxCallback(std::function<void(const spdlog::details::log_msg& msg)> function) { s_SandboxCallback = function; }

		static void TestCallback(const spdlog::details::log_msg& msg)
		{
			Logger::Game(LogLevel::Trace, "callback from engine log to sandbox log");
			Logger::Game(LogLevel::Debug, msg.payload);
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_SandboxLogger;

		static std::function<void(const spdlog::details::log_msg& msg)> s_EngineCallback;
		static std::function<void(const spdlog::details::log_msg& msg)> s_SandboxCallback;
	};


}