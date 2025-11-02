#ifndef GUARD_AUDIOSOURCE_H
#define GUARD_AUDIO_SOURCE_H

#include "../../PCH.h"

/**
 * @class AudioSource
 *
 * @brief 오디오 소스를 정의합니다.
 */
class AudioSource
{
public:
    /**
     * @brief 생성자.
     */
    AudioSource();

    /**
     * @brief 소멸자.
     */
    ~AudioSource();

    /**
     * @brief 오디오 소스를 초기화합니다.
     *
     * @return bool 초기화에 성공했다면 true, 아니면 false.
     */
    bool Initialize();

    /**
     * @brief 오디오 소스를 재생합니다.
     */
    void Play();

    /**
     * @brief 오디오 소스를 정지합니다.
     */
    void Stop();

private:
    /**
     * @brief 오디오 소스의 ID.
     */
    unsigned int id;
};

#endif // !GUARD_AUDIO_SOURCE_H