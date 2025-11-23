#pragma once

#include "../PCH.h"

class Application;

/**
 * @class Time
 * 
 * @brief 시간 관련 동작을 정의합니다.
 */
class Time final
{
    friend class Application;

public:
    /**
     * @brief 시간 배율을 반환합니다.
     * 
     * @return float 시간 배율.
     */
    [[nodiscard]]
    static inline float GetTimeScale() noexcept;

    /**
     * @brief 델타 타임(이전 프레임과 현재 프레임 사이의 시간 간격)을 초 단위로 반환합니다.
     * 
     * @return float 델타 타임(초 단위).
     */
    [[nodiscard]]
    static inline float GetDeltaTime() noexcept;

    /**
     * @brief 시간 배율이 적용되지 않은 델타 타임을 초 단위로 반환합니다.
     * 
     * @return float 시간 배율이 적용되지 않은 델타 타임(초 단위).
     */
    [[nodiscard]]
    static inline float GetUnscaledDeltaTime() noexcept;

    /**
     * @brief 시간 배율을 설정합니다.
     * 
     * @param value_ 설정할 시간 배율.
     */
    static inline void SetTimeScale(const float value_) noexcept;

private:
    /**
     * @brief 시간을 초기화합니다.
     */
    static void Initialize() noexcept;

    /**
     * @brief 시간을 업데이트합니다.
     */
    static void Update() noexcept;

    /**
     * @brief 시간 배율.
     */
    static volatile float timeScale;

    /**
     * @brief 이전 프레임의 시간.
     */
    static volatile float lastTime;

    /**
     * @brief 현재 프레임의 시간.
     */
    static volatile float currentTime;

};

inline float Time::GetTimeScale() noexcept
{
    return timeScale;
}

inline float Time::GetDeltaTime() noexcept
{
    return (currentTime - lastTime) * timeScale;
}

inline float Time::GetUnscaledDeltaTime() noexcept
{
    return currentTime - lastTime;
}

inline void Time::SetTimeScale(const float value_) noexcept
{
    timeScale = value_;
}