#include "Time.h"

void Time::Initialize() noexcept
{
    lastTime = static_cast<float>(glfwGetTime());
}

void Time::Update() noexcept
{
    currentTime = static_cast<float>(glfwGetTime());
    lastTime    = currentTime;
}

volatile float Time::timeScale = 1.0f;

volatile float Time::lastTime = 0.0f;

volatile float Time::currentTime = 0.0f;