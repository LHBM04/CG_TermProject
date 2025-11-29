#include "Input.h"

#include <GLFW/glfw3.h>

void Input::Update() noexcept
{
    lastKeyStates         = nowKeyStates;
    lastMouseButtonStates = nowMouseButtonStates;
    lastMousePosition     = nowMousePosition;
    lastScrollOffset      = nowScrollOffset;
}

void Input::OnKeyInteracted(i32 key_, i32 scancode_, i32 action_, i32 mods_) noexcept
{
    if (key_ < 0 || key_ >= MAX_KEYS)
    {
        return;
    }

    switch (action_)
    {
        case GLFW_PRESS:
        {
            nowKeyStates[static_cast<Keyboard>(key_)] = true;
            break;
        }
        case GLFW_RELEASE:
        {
            nowKeyStates[static_cast<Keyboard>(key_)] = false;
            break;
        }
        case GLFW_REPEAT:
            [[fallthrough]];
        default:
        {
            break;
        }
    }
}

void Input::OnMouseButtonInteracted(i32 button_, i32 action_, i32 mods_) noexcept
{
    if (button_ < 0 || button_ >= MAX_BUTTONS)
    {
        return;
    }

    switch (action_)
    {
        case GLFW_PRESS:
        {
            nowMouseButtonStates[static_cast<Mouse>(button_)] = true;
            break;
        }
        case GLFW_RELEASE:
        {
            nowMouseButtonStates[static_cast<Mouse>(button_)] = false;
            break;
        }
        case GLFW_REPEAT:
            [[fallthrough]];
        default:
        {
            break;
        }
    }
}

void Input::OnMouseMoved(f64 x_, f64 y_) noexcept
{
    nowMousePosition = glm::vec2(static_cast<f32>(x_), static_cast<f32>(y_));
}

void Input::OnMouseScrolled(f64 x_, f64 y_) noexcept
{
    nowScrollOffset = static_cast<f32>(y_);
}

HashMap<Keyboard, bool> Input::lastKeyStates;

HashMap<Keyboard, bool> Input::nowKeyStates;

HashMap<Mouse, bool> Input::lastMouseButtonStates;

HashMap<Mouse, bool> Input::nowMouseButtonStates;

FVector2 Input::lastMousePosition = FVector2(0.0f, 0.0f);

FVector2 Input::nowMousePosition = FVector2(0.0f, 0.0f);

f32 Input::lastScrollOffset = 0.0f;

f32 Input::nowScrollOffset = 0.0f;