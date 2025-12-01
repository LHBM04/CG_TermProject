#include "Time.h"

void TimeManager::Initialize() noexcept
{
	lastTime    = 0.0f;
	currentTime = static_cast<std::float_t>(glfwGetTime());
}

void TimeManager::Update() noexcept
{
	lastTime    = currentTime;
	currentTime = static_cast<std::float_t>(glfwGetTime());
}

float TimeManager::timeScale = 1.0f;

float TimeManager::lastTime = 0.0f;

float TimeManager::currentTime = 0.0f;