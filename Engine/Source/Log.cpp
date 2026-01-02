#include "BsPrecompileHeader.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/callback_sink.h>

namespace Pixie
{
	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_SandboxLogger;
	std::function<void(const spdlog::details::log_msg& msg)> Logger::s_Callback = &Logger::TestCallback;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("PIXIE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_SandboxLogger = spdlog::stdout_color_mt("SANDBOX");
		s_SandboxLogger->set_level(spdlog::level::trace);


		auto callback_sink = std::make_shared<spdlog::sinks::callback_sink_mt>(s_Callback);
		callback_sink->set_level(spdlog::level::err);

		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		spdlog::logger logger("custom_callback_logger", { console_sink, callback_sink });

		logger.info("some info log");
		logger.error("critical issue"); // will notify you

	}
}