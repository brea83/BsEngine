#pragma once
#define BIND_EVENT_FUNCTION(x) [this](auto&&... args) -> decltype(auto){ return this->x(std::forward<decltype(args)>(args)...);}

#define LOG_TRACE Pixie::LogLevel::Trace
#define LOG_INFO Pixie::LogLevel::Info
#define LOG_DEBUG Pixie::LogLevel::Debug
#define LOG_WARNING Pixie::LogLevel::Warning
#define LOG_ERROR Pixie::LogLevel::Error
#define LOG_CRITICAL Pixie::LogLevel::Critical