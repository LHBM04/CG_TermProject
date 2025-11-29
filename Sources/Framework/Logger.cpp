#include "Logger.h"

void Logger::Initialize() noexcept
{
    if (logger)
    {
        return;
    }

    auto consoleSink = MakeShared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_pattern("%^[%T] [%l] %v%$");

    auto fileSink = MakeShared<spdlog::sinks::basic_file_sink_mt>("logs/engine.log", true);
    fileSink->set_pattern("%^[%T] [%l] %v%$");

    List<spdlog::sink_ptr> sinks{consoleSink, fileSink};

    logger = MakeShared<spdlog::logger>("Labyrinth", sinks.begin(), sinks.end());
#if defined(DEBUG) || defined(_DEBUG)
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
#else
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);
#endif
}

Shared<spdlog::logger> Logger::logger = nullptr;