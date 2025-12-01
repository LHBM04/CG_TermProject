#pragma once

#include "Common.h"

class Application;

/**
 * @class TimeManager
 *
 * @brief 시간 관련 동작을 정의합니다.
 */
class TimeManager final
{
    friend class Application;

    STATIC_CLASS(TimeManager)
public:
    /**
     * @brief 시간 배율을 반환합니다.
     *
     * @return float 시간 배율.
     */
    [[nodiscard]]
    static inline float GetTimeScale() noexcept
    {
        return timeScale;
    }

    /**
     * @brief 시간 배율을 설정합니다.
     *
     * @param value_ 설정할 시간 배율.
     */
    static inline void SetTimeScale(const float value_) noexcept
    {
        timeScale = value_;
    }

    /**
     * @brief 델타 타임(이전 프레임과 현재 프레임 사이의 시간 간격)을 초 단위로 반환합니다.
     *
     * @return float 델타 타임(초 단위).
     */
    [[nodiscard]]
    static inline float GetDeltaTime() noexcept
    {
        return (currentTime - lastTime) * timeScale;
    }

    /**
     * @brief 시간 배율이 적용되지 않은 델타 타임을 초 단위로 반환합니다.
     *
     * @return float 시간 배율이 적용되지 않은 델타 타임(초 단위).
     */
    [[nodiscard]]
    static inline float GetUnscaledDeltaTime() noexcept
    {
        return currentTime - lastTime;
    }

    /**
     * @brief 고정 델타 타임을 반환합니다.
     *
     * @return float 고정 델타 타임.
     */
    [[nodiscard]]
    static inline float GetFixedDeltaTime() noexcept
    {
        return 1.0f / 60.0f;
    }

    /**
     * @brief 시간 배율이 적용되지 않은 고정 델타 타임을 반환합니다.
     *
     * @return float 시간 배율이 적용되지 않은 고정 델타 타임.
     */
    [[nodiscard]]
    static inline float GetFixedUnscaledDeltaTime() noexcept
    {
        return 1.0f / 60.0f;
    }

private:
    /**
     * @brief 
     */
    static void Initialize() noexcept;

    /**
     * @brief 시간을 업데이트합니다.
     */
    static void Update() noexcept;

    /**
     * @brief 시간 배율.
     */
    static float timeScale;

    /**
     * @brief 이전 프레임의 시간.
     */
    static float lastTime;

    /**
     * @brief 현재 프레임의 시간.
     */
    static float currentTime;
};