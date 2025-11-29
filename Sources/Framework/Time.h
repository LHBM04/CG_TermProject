#pragma once

#include "Common.h"

class Application;

/**
 * @class Time
 *
 * @brief 시간 관련 동작을 정의합니다.
 */
class Time final
{
    friend class Application;

    STATIC_CLASS(Time)
public:
    /**
     * @brief 시간 배율을 반환합니다.
     *
     * @return float 시간 배율.
     */
    [[nodiscard]]
    static inline f32 GetTimeScale() noexcept;

    /**
     * @brief 델타 타임(이전 프레임과 현재 프레임 사이의 시간 간격)을 초 단위로 반환합니다.
     *
     * @return float 델타 타임(초 단위).
     */
    [[nodiscard]]
    static inline f32 GetDeltaTime() noexcept;

    /**
     * @brief 시간 배율이 적용되지 않은 델타 타임을 초 단위로 반환합니다.
     *
     * @return float 시간 배율이 적용되지 않은 델타 타임(초 단위).
     */
    [[nodiscard]]
    static inline f32 GetUnscaledDeltaTime() noexcept;

    /**
     * @brief 시간 배율을 설정합니다.
     *
     * @param value_ 설정할 시간 배율.
     */
    static inline void SetTimeScale(f32 value_) noexcept;

private:
    /**
     * @brief 시간을 업데이트합니다.
     */
    static void Update() noexcept;

    /**
     * @brief 시간 배율.
     */
    static f32 timeScale;

    /**
     * @brief 이전 프레임의 시간.
     */
    static f32 lastTime;

    /**
     * @brief 현재 프레임의 시간.
     */
    static f32 currentTime;
};

inline f32 Time::GetTimeScale() noexcept
{
    return timeScale;
}

inline f32 Time::GetDeltaTime() noexcept
{
    return (currentTime - lastTime) * timeScale;
}

inline f32 Time::GetUnscaledDeltaTime() noexcept
{
    return currentTime - lastTime;
}

inline void Time::SetTimeScale(f32 value_) noexcept
{
    timeScale = value_;
}