#include "Time.h"

#include <GLFW/glfw3.h>

void Time::Initialize() noexcept
{
    currentTime = static_cast<float>(glfwGetTime());
    lastTime    = currentTime;
}

void Time::Update() noexcept
{
    lastTime    = currentTime;
    currentTime = static_cast<float>(glfwGetTime());
}

volatile float Time::timeScale = 1.0f;

volatile float Time::lastTime = 0.0f;

volatile float Time::currentTime = 0.0f;