#pragma once

#include <glm/fwd.hpp>

namespace Input
{
    /**
     * @enum Keyboard
     *
     * @brief 키 코드를 정의합니다.
     */
    enum class Keyboard : unsigned short
    {
        Space      = 32,
        Apostrophe = 39, /* ' */
        Comma      = 44, /* , */
        Minus      = 45, /* - */
        Period     = 46, /* . */
        Slash      = 47, /* / */

        D0 = 48, /* 0 */
        D1 = 49, /* 1 */
        D2 = 50, /* 2 */
        D3 = 51, /* 3 */
        D4 = 52, /* 4 */
        D5 = 53, /* 5 */
        D6 = 54, /* 6 */
        D7 = 55, /* 7 */
        D8 = 56, /* 8 */
        D9 = 57, /* 9 */

        Semicolon = 59, /* ; */
        Equal     = 61, /* = */

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        LeftBracket  = 91, /* [ */
        Backslash    = 92, /* \ */
        RightBracket = 93, /* ] */
        GraveAccent  = 96, /* ` */

        World1 = 161,
        World2 = 162,

        Escape      = 256,
        Enter       = 257,
        Tab         = 258,
        Backspace   = 259,
        Insert      = 260,
        Delete      = 261,
        Right       = 262,
        Left        = 263,
        Down        = 264,
        Up          = 265,
        PageUp      = 266,
        PageDown    = 267,
        Home        = 268,
        End         = 269,
        CapsLock    = 280,
        ScrollLock  = 281,
        NumLock     = 282,
        PrintScreen = 283,
        Pause       = 284,
        F1          = 290,
        F2          = 291,
        F3          = 292,
        F4          = 293,
        F5          = 294,
        F6          = 295,
        F7          = 296,
        F8          = 297,
        F9          = 298,
        F10         = 299,
        F11         = 300,
        F12         = 301,
        F13         = 302,
        F14         = 303,
        F15         = 304,
        F16         = 305,
        F17         = 306,
        F18         = 307,
        F19         = 308,
        F20         = 309,
        F21         = 310,
        F22         = 311,
        F23         = 312,
        F24         = 313,
        F25         = 314,

        KP0        = 320,
        KP1        = 321,
        KP2        = 322,
        KP3        = 323,
        KP4        = 324,
        KP5        = 325,
        KP6        = 326,
        KP7        = 327,
        KP8        = 328,
        KP9        = 329,
        KPDecimal  = 330,
        KPDivide   = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd      = 334,
        KPEnter    = 335,
        KPEqual    = 336,

        LeftShift    = 340,
        LeftControl  = 341,
        LeftAlt      = 342,
        LeftSuper    = 343,
        RightShift   = 344,
        RightControl = 345,
        RightAlt     = 346,
        RightSuper   = 347,
        Menu         = 348
    };

    /**
     * @enum Mouse
     *
     * @brief 마우스 버튼 코드를 정의합니다.
     */
    enum class Mouse : unsigned char
    {
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,

        ButtonLast   = Button7,
        ButtonLeft   = Button0,
        ButtonRight  = Button1,
        ButtonMiddle = Button2
    };

    /**
     * @brief 지정한 키가 눌렸는지 여부를 반환합니다.
     *
     * @param key_ 지정할 키
     *
     * @return bool 키가 눌렸는지 여부
     */
    [[nodiscard]]
    bool IsKeyPressed(const Input::Keyboard key_) noexcept;

    /*
     * @brief 지정한 키가 떼어졌는지 여부를 반환합니다.
     *
     * @param key_ 지정할 키
     *
     * @return bool 키가 떼어졌는지 여부
     */
    [[nodiscard]]
    bool IsKeyReleased(const Input::Keyboard key_) noexcept;

    /**
     * @brief 지정한 키가 눌려져 있는지 여부를 반환합니다.
     *
     * @param key_ 지정할 키
     *
     * @return bool 키가 눌려져 있는지 여부
     */
    [[nodiscard]]
    bool IsKeyHeld(const Input::Keyboard key_) noexcept;

    /**
     * @brief 지정한 마우스 버튼이 눌렸는지 여부를 반환합니다.
     *
     * @param button_ 지정할 마우스 버튼
     *
     * @return bool 마우스 버튼이 눌렸는지 여부
     */
    [[nodiscard]]
    bool IsMouseButtonPressed(const Input::Mouse button_) noexcept;

    /**
     * @brief 지정한 마우스 버튼이 떼어졌는지 여부를 반환합니다.
     *
     * @param button_ 지정할 마우스 버튼
     *
     * @return bool 마우스 버튼이 떼어졌는지 여부
     */
    [[nodiscard]]
    bool IsMouseButtonReleased(const Input::Mouse button_) noexcept;

    /**
     * @brief 지정한 마우스 버튼이 눌려져 있는지 여부를 반환합니다.
     *
     * @param button_ 지정할 마우스 버튼
     *
     * @return bool 마우스 버튼이 눌려져 있는지 여부
     */
    [[nodiscard]]
    bool IsMouseButtonHeld(const Input::Mouse button_) noexcept;

    /**
     * @brief 현재 마우스 위치를 반환합니다.
     *
     * @return glm::vec2 현재 마우스 위치
     */
    [[nodiscard]]
    glm::fvec2 GetMousePosition() noexcept;

    /**
     * @brief 마우스 위치의 변화량을 반환합니다.
     *
     * @return glm::vec2 마우스 위치의 변화량
     */
    [[nodiscard]]
    glm::fvec2 GetMousePositionDelta() noexcept;

    /**
     * @brief 마우스 스크롤 오프셋 값을 반환합니다.
     *
     * @return float 마우스 스크롤 오프셋 값
     */
    [[nodiscard]]
    float GetScrollOffset() noexcept;

    /**
     * @brief 마우스 스크롤의 변화량을 반환합니다.
     *
     * @return float 마우스 스크롤의 변화량
     */
    [[nodiscard]]
    float GetScrollOffsetDelta() noexcept;
} // namespace Input