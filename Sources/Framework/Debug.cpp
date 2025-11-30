#include "Debug.h"

void Logger::Initialize() noexcept
{
    if (logger)
    {
        return;
    }

    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_pattern("%^[%T] [%l] [%s:%#] %v%$");

    auto    now   = std::chrono::system_clock::now();
    auto    timeT = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
#if defined(_WIN32)
    localtime_s(&localTime, &timeT);
#else
    localtime_r(&timeT, &localTime);
#endif

    std::ostringstream oss;
    oss << "Logs/" << std::put_time(&localTime, "%Y%m%d_%H%M%S") << ".log";
    std::string logFileName = oss.str();

    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFileName, true);
    fileSink->set_pattern("%^[%T] [%l] [%s:%#] %v%$");

    std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};

    logger = std::make_shared<spdlog::logger>("Labyrinth Log", sinks.begin(), sinks.end());
#if defined(DEBUG) || defined(_DEBUG)
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
#else
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);
#endif
}

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;