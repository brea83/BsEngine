#include "BsPrecompileHeader.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace Pixie
{
	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_SandboxLogger;
	std::function<void(const spdlog::details::log_msg& msg)> Logger::s_EngineCallback = &Logger::TestCallback;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		std::vector<spdlog::sink_ptr> logSinks;
		// sink that writes to console with colors for each level
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		// Create a file rotating sink with 5 MB size max and 3 rotated files
		auto maxSize = 1048576 * 5;
		auto maxFiles = 3;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/Pixie_Log.txt", maxSize, maxFiles));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("PIXIE", begin(logSinks), end(logSinks));
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_SandboxLogger = std::make_shared<spdlog::logger>("SANDBOX", begin(logSinks), end(logSinks));
		s_SandboxLogger->set_level(spdlog::level::trace);
		s_SandboxLogger->flush_on(spdlog::level::trace);

		Logger::Log(Pixie::LogLevel::Trace, "====================================================================");
		Logger::Log(Pixie::LogLevel::Trace, "EngineContext::Init() -> Logger Initialized");
		Logger::Log(Pixie::LogLevel::Trace, "====================================================================");
	}
}