#include "Input.h"

void InputManager::Initialize(GLFWwindow* const window_) noexcept
{
    window = window_;
    glfwSetKeyCallback(window_, InputManager::OnKeyInteracted);
    glfwSetMouseButtonCallback(window_, InputManager::OnMouseButtonInteracted);
    glfwSetCursorPosCallback(window_, InputManager::OnMouseMoved);
    glfwSetScrollCallback(window_, InputManager::OnMouseScrolled);
}

void InputManager::Update() noexcept
{
    lastKeyStates         = nowKeyStates;
    lastMouseButtonStates = nowMouseButtonStates;
    lastMousePosition     = nowMousePosition;
    lastScrollOffset      = nowScrollOffset;
}

void InputManager::OnKeyInteracted(GLFWwindow* const window_, int key_, int scancode_, int action_, int mods_) noexcept
{
    if (key_ < 0 || key_ >= MAX_KEYS)
    {
        return;
    }

    switch (action_)
    {
        case GLFW_PRESS:
        {
            nowKeyStates[key_] = true;
            break;
        }
        case GLFW_RELEASE:
        {
            nowKeyStates[key_] = false;
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

void InputManager::OnMouseButtonInteracted(GLFWwindow* const window_, int button_, int action_, int mods_) noexcept
{
    if (button_ < 0 || button_ >= MAX_MOUSE_BUTTONS)
    {
        return;
    }

    switch (action_)
    {
        case GLFW_PRESS:
        {
            nowMouseButtonStates[button_] = true;
            break;
        }
        case GLFW_RELEASE:
        {
            nowMouseButtonStates[button_] = false;
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

void InputManager::OnMouseMoved(GLFWwindow* const window_, double x_, double y_) noexcept
{
    nowMousePosition = glm::vec2(static_cast<float>(x_), static_cast<float>(y_));
}

void InputManager::OnMouseScrolled(GLFWwindow* const window_, double x_, double y_) noexcept
{
    nowScrollOffset = static_cast<float>(y_);
}

GLFWwindow* InputManager::window = nullptr;

std::array<bool, InputManager::MAX_KEYS> InputManager::lastKeyStates;

std::array<bool, InputManager::MAX_KEYS> InputManager::nowKeyStates;

std::array<bool, InputManager::MAX_MOUSE_BUTTONS> InputManager::lastMouseButtonStates;

std::array<bool, InputManager::MAX_MOUSE_BUTTONS> InputManager::nowMouseButtonStates;

glm::fvec2 InputManager::lastMousePosition = glm::fvec2(0.0f, 0.0f);

glm::fvec2 InputManager::nowMousePosition = glm::fvec2(0.0f, 0.0f);

float InputManager::lastScrollOffset = 0.0f;

float InputManager::nowScrollOffset = 0.0f;