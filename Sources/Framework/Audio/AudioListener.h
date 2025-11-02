#ifndef GUARD_AUDIOLISTENER_H
#define GUARD_AUDIOLISTENER_H

#include "../../PCH.h"

class AudioListener
{
public:
    /**
     * @brief 생성자.
     */
    explicit AudioListener() noexcept;

    /**
     * @brief 소멸자.
     */
    ~AudioListener() noexcept;

    /**
     * @brief 오디오 리스너를 초기화합니다.
     *
     * @return bool 초기화에 성공했다면 true, 아니면 false.
     */
    bool Initialize();
};

#endif // !GUARD_AUDIOLISTENER_H