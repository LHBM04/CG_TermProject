#include "Time.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void Time::Update() noexcept
{
	lastTime    = currentTime;
	currentTime = static_cast<f32>(glfwGetTime());
}

float Time::timeScale = 1.0f;

float Time::lastTime = 0.0f;

float Time::currentTime = 0.0f;