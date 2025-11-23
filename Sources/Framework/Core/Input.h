#pragma once

#include "../PCH.h"

class Application;

/**
 * @class Input
 * 
 * @brief 키/입력 관련 동작을 정의합니다.
 */
class Input final
{
    friend class Application;

public:
    /**
     * @brief 지정한 키가 눌렸는지 여부를 반환합니다.
     * 
     * @param key_ 지정할 키
     * 
     * @return bool 키가 눌렸는지 여부
     */
    [[nodiscard]]
    static inline bool IsKeyPressed(int key_) noexcept;

    /*
     * @brief 지정한 키가 떼어졌는지 여부를 반환합니다.
     * 
     * @param key_ 지정할 키
     * 
     * @return bool 키가 떼어졌는지 여부
     */
    [[nodiscard]]
    static inline bool IsKeyReleased(int key_) noexcept;

    /**
     * @brief 지정한 키가 눌려져 있는지 여부를 반환합니다.
     * 
     * @param key_ 지정할 키
     * 
     * @return bool 키가 눌려져 있는지 여부
     */
    [[nodiscard]]
    static inline bool IsKeyHeld(int key_) noexcept;

    /**
     * @brief 지정한 마우스 버튼이 눌렸는지 여부를 반환합니다.
     * 
     * @param button_ 지정할 마우스 버튼
     * 
     * @return bool 마우스 버튼이 눌렸는지 여부
     */
    [[nodiscard]]
    static inline bool IsMouseButtonPressed(int button_) noexcept;

    /**
     * @brief 지정한 마우스 버튼이 떼어졌는지 여부를 반환합니다. 
     * 
     * @param button_ 지정할 마우스 버튼
     * 
     * @return bool 마우스 버튼이 떼어졌는지 여부
     */
    [[nodiscard]]
    static inline bool IsMouseButtonReleased(int button_) noexcept;

    /**
     * @brief 지정한 마우스 버튼이 눌려져 있는지 여부를 반환합니다.
     * 
     * @param button_ 지정할 마우스 버튼
     * 
     * @return bool 마우스 버튼이 눌려져 있는지 여부
     */
    [[nodiscard]]
    static inline bool IsMouseButtonHeld(int button_) noexcept;

    /**
     * @brief 현재 마우스 위치를 반환합니다.
     * 
     * @return glm::vec2 현재 마우스 위치
     */
    [[nodiscard]]
    static inline glm::vec2 GetMousePosition() noexcept;

    /**
     * @brief 마우스 위치의 변화량을 반환합니다.
     * 
     * @return glm::vec2 마우스 위치의 변화량
     */
    [[nodiscard]]
    static inline glm::vec2 GetMousePositionDelta() noexcept;

    /**
     * @brief 마우스 스크롤 오프셋 값을 반환합니다.
     * 
     * @return float 마우스 스크롤 오프셋 값
     */
    [[nodiscard]]
    static inline float GetScrollOffset() noexcept;

    /**
     * @brief 마우스 스크롤의 변화량을 반환합니다.
     * 
     * @return float 마우스 스크롤의 변화량
     */
    [[nodiscard]]
    static inline float GetScrollOffsetDelta() noexcept;

private:
    /**
     * @brief 입력 관련 콜백을 등록하고, 입력 상태를 초기화합니다.
     * 
     * @param window_ 애플리케이션의 네이티브 윈도우 핸들
     */
    static void Initialize(GLFWwindow* const window_) noexcept;

    /**
     * @brief 입력 상태를 업데이트합니다.
     */
    static void Update() noexcept;

    /**
     * @brief 키 상호작용이 발생했을 때 호출됩니다.
     * 
     * @param key_      눌린 키
     * @param scancode_ 스캔 코드
     * @param action_   키 액션
     * @param mods_     키 상태
     */
    static void OnKeyInteracted(int key_, 
                                int scancode_, 
                                int action_, 
                                int mods_) noexcept;

    /**
     * @brief 마우스 버튼 상호작용이 발생했을 때 호출됩니다.
     * 
     * @param button_ 눌린 버튼
     * @param action_ 버튼 액션
     * @param mods_   버튼 상태
     */
    static void OnMouseButtonInteracted(int button_, 
                                        int action_, 
                                        int mods_) noexcept;

    /**
     * @brief 마우스 이동이 발생했을 때 호출됩니다.
     * 
     * @param x_ 마우스 X 좌표
     * @param y_ 마우스 Y 좌표
     */
    static void OnMouseMoved(double x_, 
                             double y_) noexcept;

    /**
     * @brief 마우스 스크롤이 발생했을 때 호출됩니다.
     * 
     * @param x_ 마우스 X 좌표
     * @param y_ 마우스 Y 좌표
     */
    static void OnMouseScrolled(double x_, 
                                double y_) noexcept;

    /**
     * @brief 키 최대 개수.
     */
    static constexpr std::size_t MAX_KEYS = GLFW_KEY_LAST;

    /**
     * @brief 버튼 최대 개수.
     */
    static constexpr std::size_t MAX_MOUSE_BUTTONS = GLFW_MOUSE_BUTTON_LAST;
    
    /**
     * @brief 이전 프레임에서의 키 상태.
     */
    static std::array<bool, MAX_KEYS> lastKeyStates;

    /**
     * @brief 현재 프레임에서의 키 상태.
     */
    static std::array<bool, MAX_KEYS> nowKeyStates;

    /**
     * @brief 이전 프레임에서의 마우스 버튼 상태.
     */
    static std::array<bool, MAX_MOUSE_BUTTONS> lastMouseButtonStates;

    /**
     * @brief 현재 프레임에서의 마우스 버튼 상태.
     */
    static std::array<bool, MAX_MOUSE_BUTTONS> nowMouseButtonStates;

    /**
     * @brief 이전 프레임에서의 마우스 위치.
     */
    static glm::vec2 lastMousePosition;

    /**
     * @brief 현재 프레임에서의 마우스 위치.
     */
    static glm::vec2 nowMousePosition;

    /**
     * @brief 이전 프레임에서의 스크롤 값.
     */
    static volatile float lastScrollOffset;

    /**
     * @brief 현재 프레임에서의 스크롤 값.
     */
    static volatile float nowScrollOffset;
};

inline bool Input::IsKeyPressed(int key_) noexcept
{
    return nowKeyStates[key_] && !lastKeyStates[key_];
}

inline bool Input::IsKeyReleased(int key_) noexcept
{
    return !nowKeyStates[key_] && lastKeyStates[key_];
}

inline bool Input::IsKeyHeld(int key_) noexcept
{
    return nowKeyStates[key_] && lastKeyStates[key_];
}

inline bool Input::IsMouseButtonPressed(int button_) noexcept
{
    return nowMouseButtonStates[button_] && !lastMouseButtonStates[button_];
}

inline bool Input::IsMouseButtonReleased(int button_) noexcept
{
    return !nowMouseButtonStates[button_] && lastMouseButtonStates[button_];
}

inline bool Input::IsMouseButtonHeld(int button_) noexcept
{
    return nowMouseButtonStates[button_] && lastMouseButtonStates[button_];
}

inline glm::vec2 Input::GetMousePosition() noexcept
{
    return nowMousePosition;
}

inline glm::vec2 Input::GetMousePositionDelta() noexcept
{
    return nowMousePosition - lastMousePosition;
}

inline float Input::GetScrollOffset() noexcept
{
    return nowScrollOffset;
}

inline float Input::GetScrollOffsetDelta() noexcept
{
    return nowScrollOffset - lastScrollOffset;
}