#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "Common.h"

#define LOG_TRACE(message_, ...) Logger::Trace(message_, ...)
#define LOG_INFO(message_, ...) Logger::Info(message_, ...)
#define LOG_WARNING(message_, ...) Logger::Warning(message_, ...)
#define LOG_ERROR(message_, ...) Logger::Error(message_, ...)
#define LOG_CRITICAL(mes) Logger::Critical(message_, ...)

/**
 * @class Logger
 *
 * @brief 로거를 정의합니다.
 */
class Logger final
{
    STATIC_CLASS(Logger)

public:
    /**
     * @brief 로거를 초기화합니다.
     */
    static void Initialize() noexcept;

    /**
     * @brief 추적 로그를 출력합니다.
     *
     * @tparam ...Args 출력 메시지에 포함될 인자
     *
     * @param message_ 출력할 메시지
     * @param ...args_ 출력할 메시지에 포함될 인자
     */
    template <typename... Args>
    static void Trace(spdlog::format_string_t<Args...> message_, Args... args_)
    {
        if (!logger)
        {
            throw std::runtime_error("");
        }

        logger->trace(message_, std::forward<Args>(args_)...);
    }

    /**
     * @brief 알림 로그를 출력합니다.
     *
     * @tparam ...Args 출력 메시지에 포함될 인자
     *
     * @param message_ 출력할 메시지
     * @param ...args_ 출력할 메시지에 포함될 인자
     */
    template <typename... Args>
    static void Info(spdlog::format_string_t<Args...> message_, Args... args_)
    {
        if (!logger)
        {
            throw std::runtime_error("");
        }

        logger->info(message_, std::forward<Args>(args_)...);
    }

    /**
     * @brief 경고 로그를 출력합니다.
     *
     * @tparam ...Args 출력 메시지에 포함될 인자
     *
     * @param message_ 출력할 메시지
     * @param ...args_ 출력할 메시지에 포함될 인자
     */
    template <typename... Args>
    static void Warning(spdlog::format_string_t<Args...> message_, Args... args_)
    {
        if (!logger)
        {
            throw std::runtime_error("");
        }

        logger->warn(message_, std::forward<Args>(args_)...);
    }

    /**
     * @brief 오류 로그를 출력합니다.
     *
     * @tparam ...Args 출력 메시지에 포함될 인자
     *
     * @param message_ 출력할 메시지
     * @param ...args_ 출력할 메시지에 포함될 인자
     */
    template <typename... Args>
    static void Error(spdlog::format_string_t<Args...> message_, Args... args_)
    {
        if (!logger)
        {
            throw std::runtime_error("");
        }
        logger->error(message_, std::forward<Args>(args_)...);
    }

    /**
     * @brief 치명적 오류 로그를 출력합니다.
     *
     * @tparam ...Args 출력 메시지에 포함될 인자
     *
     * @param message_ 출력할 메시지
     * @param ...args_ 출력할 메시지에 포함될 인자
     */
    template <typename... Args>
    static void Critical(spdlog::format_string_t<Args...> message_, Args... args_)
    {
        if (!logger)
        {
            throw std::runtime_error("");
        }

        logger->critical(message_, std::forward<Args>(args_)...);
    }

private:
    /**
     * @enum Level
     *
     * @brief 로그 레벨을 정의합니다.
     */
    enum class Level : unsigned char
    {
        /**
         * @brief 추적 로그.
         */
        Trace,

        /**
         * @brief 알림 로그.
         */
        Info,

        /**
         * @brief 경고 로그.
         */
        Warning,

        /**
         * @brief 에러 로그. (하지만 실행은 될 수 있는 수준)
         */
        Error,

        /**
         * @brief 에러 로그. (실행이 불가능한 수준)
         */
        Critical,
    };

    /**
     * @brief 사용할 로거.
     */
    static std::shared_ptr<spdlog::logger> logger;
};