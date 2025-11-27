#pragma once

#include <stdexcept>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

class AudioSystem
{
private:
    ALCdevice*  device  = nullptr;
    ALCcontext* context = nullptr;

public:
    AudioSystem()
    {
        // 1. 기본 오디오 장치 열기 (NULL은 기본 장치 요청) [cite: 76-77]
        device = alcOpenDevice(nullptr);
        if (!device)
        {
            throw std::runtime_error("OpenAL 디바이스를 열 수 없습니다.");
        }

        // 2. 컨텍스트 생성 [cite: 106]
        context = alcCreateContext(device, nullptr);
        if (!context)
        {
            alcCloseDevice(device);
            throw std::runtime_error("OpenAL 컨텍스트를 생성할 수 없습니다.");
        }

        // 3. 현재 스레드에 컨텍스트 활성화 [cite: 107]
        if (!alcMakeContextCurrent(context))
        {
            alcDestroyContext(context);
            alcCloseDevice(device);
            throw std::runtime_error("OpenAL 컨텍스트를 활성화할 수 없습니다.");
        }

        std::cout << "Audio System Initialized." << std::endl;
    }

    ~AudioSystem()
    {
        alcMakeContextCurrent(nullptr);
        if (context)
            alcDestroyContext(context);
        if (device)
            alcCloseDevice(device);

        std::cout << "Audio System Shutdown." << std::endl;
    }

    // 엔진의 Update 루프 등에서 에러 체크용 유틸리티
    static void CheckError(const std::string& message)
    {
        ALenum error = alGetError(); // 에러 상태를 가져오고 초기화함 [cite: 1967]
        if (error != AL_NO_ERROR)
        {
            std::cerr << "[OpenAL Error] " << message << ":Code " << error << std::endl;
        }
    }
};