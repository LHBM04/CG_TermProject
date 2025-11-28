#include "Common.h"

void Logger::Log(Level level_, const std::string& message_) noexcept
{
	static auto logger = spdlog::stdout_color_mt("console");

	switch (level_)
	{
	case Level::Trace:
		logger->trace(message_);
		break;
	case Level::Info:
		logger->info(message_);
		break;
	case Level::Warning:
		logger->warn(message_);
		break;
	case Level::Error:
		logger->error(message_);
		break;
	default:
		logger->info(message_);
		break;
	}
}