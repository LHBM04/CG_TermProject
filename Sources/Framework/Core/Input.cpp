#include "Input.h"

#include <array>
#include <cstddef>

#include <glm/gtc/type_precision.hpp>

namespace
{
    /**
     * @brief 애플리케이션에서 처리할 수 있는 키의 최대 개수.
     */
    constexpr std::size_t MAX_KEYS = static_cast<std::size_t>(Input::Keyboard::Menu) + 1;

    /**
     * @brief 애플리케이션에서 처리할 수 있는 마우스 버튼의 최대 개수.
     */
    constexpr std::size_t MAX_MOUSE_BUTTONS = static_cast<std::size_t>(Input::Mouse::Button7) + 1;

    /**
     * @brief 이전 프레임에서의 키 상태.
     */
    std::array<bool, MAX_KEYS> lastKeyStates;

    /**
     * @brief 현재 프레임에서의 키 상태.
     */
    std::array<bool, MAX_KEYS> nowKeyStates;

    /**
     * @brief 이전 프레임에서의 마우스 버튼 상태.
     */
    std::array<bool, MAX_MOUSE_BUTTONS> lastMouseButtonStates;

    /**
     * @brief 현재 프레임에서의 마우스 버튼 상태.
     */
    std::array<bool, MAX_MOUSE_BUTTONS> nowMouseButtonStates;

    /**
     * @brief 이전 프레임에서의 마우스 위치.
     */
    glm::fvec2 lastMousePosition;

    /**
     * @brief 현재 프레임에서의 마우스 위치.
     */
    glm::fvec2 nowMousePosition;

    /**
     * @brief 이전 프레임에서의 스크롤 값.
     */
    float lastScrollOffset;

    /**
     * @brief 현재 프레임에서의 스크롤 값.
     */
    float nowScrollOffset;
} // namespace

bool Input::IsKeyPressed(const Input::Keyboard key_) noexcept
{
    return nowKeyStates[static_cast<std::size_t>(key_)] && 
          !lastKeyStates[static_cast<std::size_t>(key_)];
}

bool Input::IsKeyReleased(const Input::Keyboard key_) noexcept
{
    return !nowKeyStates[static_cast<std::size_t>(key_)] && 
            lastKeyStates[static_cast<std::size_t>(key_)];
}

bool Input::IsKeyHeld(const Input::Keyboard key_) noexcept
{
    return nowKeyStates[static_cast<std::size_t>(key_)] && 
           lastKeyStates[static_cast<std::size_t>(key_)];
}

bool Input::IsMouseButtonPressed(const Input::Mouse mouseButton_) noexcept
{
    return nowMouseButtonStates[static_cast<std::size_t>(mouseButton_)] &&
          !lastMouseButtonStates[static_cast<std::size_t>(mouseButton_)];
}

bool Input::IsMouseButtonReleased(const Input::Mouse mouseButton_) noexcept
{
    return !nowMouseButtonStates[static_cast<std::size_t>(mouseButton_)] &&
            lastMouseButtonStates[static_cast<std::size_t>(mouseButton_)];
}

bool Input::IsMouseButtonHeld(const Input::Mouse mouseButton_) noexcept
{
    return nowMouseButtonStates[static_cast<std::size_t>(mouseButton_)] &&
           lastMouseButtonStates[static_cast<std::size_t>(mouseButton_)];
}

glm::fvec2 Input::GetMousePosition()
{
    return nowMousePosition;
}

glm::fvec2 Input::GetMousePositionDelta()
{
    return nowMousePosition - lastMousePosition;
}

float Input::GetScrollOffset()
{
    return nowScrollOffset;
}

float Input::GetScrollOffsetDelta()
{
    return nowScrollOffset - lastScrollOffset;
}