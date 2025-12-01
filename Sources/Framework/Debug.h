#pragma once

#include "Common.h"

// 강제 종료 매크로
#define ABORT(message_, ...)                                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        Logger::Critical(message_, ...);                                                                               \
        std::abort();                                                                                                  \
    }                                                                                                                  \
    while (0)

// 조건 실패 시 종료하는 ASSERT 매크로
#define ASSERT(condition_, message_, ...)                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition_))                                                                                             \
        {                                                                                                              \
            Logger::Critical(message_, ...);                                                                           \
            std::abort();                                                                                              \
        }                                                                                                              \
    }                                                                                                                  \
    while (0)                               \

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
    static void Warn(spdlog::format_string_t<Args...> message_, Args... args_)
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
        Warn,

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