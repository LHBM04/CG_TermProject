#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "Common.h"
#include "Objects.h"

class AudioClip;

namespace AudioSystem
{
    /**
     * @brief 오디오 시스템을 초기화합니다.
     */
    void Initialize();

    /**
     * @brief 오디오 시스템을 종료합니다.
     */
    void Quit();
} // namespace AudioSystem

/**
 * @class AudioSource
 *
 * @brief 오디오를 재생합니다.
 */
class AudioSource : public Component
{
public:
    /**
     * @brief 소스의 오디오 재생 레이아웃을 정의합니다.
     */
    enum class Layout : unsigned char
    {
        /**
         * @brief 모노.
         */
        Mono,

        /**
         * @brief 스테레오.
         */
        Stereo
    };

    /**
     * @brief 생성자.
     * 
     * @param owner 해당 컴포넌트의 오너
     */
    explicit AudioSource(Object* const owner) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~AudioSource() noexcept override;

    /**
     * @brief 
     */
    virtual void Awake() override;

    /**
     * @brief 
     */
    virtual void OnDestroy() override;

    /**
     * @brief 
     */
    virtual void Update() override;

    /**
     * @brief 
     */
    void Play();

    /**
     * @brief 
     */
    void Stop();

    /**
     * @brief 
     */
    void Pause();

    /**
     * @brief 해당 소스에 할당된 오디오 클립을 반환합니다.
     * 
     * @return AudioClip* 해당 소스에 할당된 오디오 클립
     */
    [[nodiscard]]
    inline AudioClip* const GetCurrentClip() const noexcept
    {
        return currentClip;
    }

    /**
     * @brief 해당 소스에 지정한 오디오 클립을 할당합니다.
     * 
     * @param audioClip_ 지정할 오디오 클립
     */
    inline void SetClip(AudioClip* audioClip_)
    {
        currentClip = audioClip_;

        if (sourceID != 0 && currentClip)
        {
            alSourcei(sourceID, AL_BUFFER, currentClip->GetBufferID());
        }
        else if (sourceID != 0)
        {
            alSourcei(sourceID, AL_BUFFER, 0);
        }
    }

    /**
     * @brief 해당 오디오의 
     * 
     * @return 
     */
    [[nodiscard]] 
    inline float GetVolume() const
    {
        return volume;
    }

    /**
     * @brief 해당 소스의 볼륨을 설정합니다.
     * 
     * @param volume_ 설정할 볼륨
     */
    inline void SetVolume(float volume_) noexcept
    {
        volume = volume_;
        alSourcef(sourceID, AL_GAIN, volume);
    }

    /**
     * @brief 해당 소스의 피치 값을 반환합니다.
     * 
     * @return float 해당 소스의 피치 값
     */
    [[nodiscard]] 
    inline float GetPitch() const noexcept
    {
        return pitch;
    }

    /**
     * @brief 해당 소스의 피치 값을 설정합니다.
     * 
     * @param pitch_ 설정할 피치 값
     */
    virtual void SetPitch(float pitch_) noexcept
    {
        pitch = pitch_;
        alSourcef(sourceID, AL_PITCH, pitch);
    }

    /**
     * @brief 해당 소스의 반복 재생 여부를 반환합니다.
     * 
     * @return bool 해당 소스의 반복 재생 여부
     */
    [[nodiscard]]
    inline bool IsLooping() const noexcept
    {
        return isLooping;
    }

    /**
     * @brief 해당 소스의 반복 재생 여부를 설정합니다.
     * 
     * @param loop_ 반복 재생 여부
     */
    inline void SetLooping(bool loop_)
    {
        isLooping = loop_;
        alSourcei(sourceID, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);
    }

private:
    /**
     * @brief 해당 오디오 소스 ID.
     */
    unsigned int sourceID;

    /**
     * @brief 해당 소스에 할당된 오디오 클립.
     */
    AudioClip* currentClip;

    /**
     * @brief Awake 실행 시점에 오디오 재생 여부.
     */
    bool playOnAwake;

    /**
     * @brief 반복 재생 여부.
     */
    bool isLooping;

    /**
     * @brief 볼륨.
     */
    float volume;

    /**
     * @brief 피치.
     */
    float pitch;
};

/**
 * @class AudioListener
 * 
 * @brief 씬 내의 "듣는 위치(귀)"를 정의하는 컴포넌트입니다.
 */
class AudioListener final : public Component
{
public:
    /**
     * @brief 생성자.
     * 
     * @param owner 해당 컴포넌트의 오너
     */
    explicit AudioListener(Object* const owner) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~AudioListener() noexcept override;

    /**
     * @brief 매 프레임 트랜스폼의 위치/회전을 OpenAL 리스너에 동기화합니다.
     */
    virtual void Update() override;
};