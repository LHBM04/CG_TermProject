#ifndef GUARD_WINDOW_H
#define GUARD_WINDOW_H

#include "../../PCH.h"

/**
 * @class Window
 *
 * @brief 애플리케이션 내 윈도우를 정의합니다.
 */
class Window
{
public:
    /**
     * @struct Specification
     *
     * @brief 윈도우의 사양을 정의합니다.
     */
    struct Specification final
    {
        /**
         * @brief 윈도우의 제목.
         */
        const char* title;

        /**
         * @brief 윈도우의 너비.
         */
        int width;

        /**
         * @brief 윈도우의 높이.
         */
        int height;

        /**
         * @brief 윈도우의 x 좌표.
         */
        int x;

        /**
         * @brief 윈도우의 y 좌표.
         */
        int y;

        /**
         * @brief 윈도우의 크기 조절 가능 여부.
         */
        bool resizable;

        /**
         * @brief 전체 화면 모드 여부.
         */
        bool fullscreen;

        /**
         * @brief 수직 동기화 여부.
         */
        bool vsync;

        /**
         * @brief 윈도우 장식 여부.
         */
        bool decorated;
    };

    /**
     * @brief 생성자.
     *
     * @param specification_ 윈도우의 사양.
     */
    explicit Window(const Window::Specification& specification_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Window() noexcept;

    /**
     * @brief 윈도우를 업데이트합니다.
     */
    void Update() noexcept;

    /**
     * @brief 네이티브 핸들을 반환합니다.
     */
    [[nodiscard]]
    inline const GLFWwindow* const GetNativeHandle() const noexcept;

    /**
     * @brief 윈도우의 사양을 반환합니다.
     */
    [[nodiscard]]
    inline const Window::Specification& GetSpecification() const noexcept;

    /**
     * @brief 윈도우가 닫혀야 하는지 여부를 반환합니다.
     */
    [[nodiscard]]
    inline bool ShouldClose() const noexcept;

private:
    /**
     * @brief 해당 윈도우의 네이티브 핸들.
     */
    GLFWwindow* window = nullptr;

    /**
     * @brief 해당 윈도우의 사양.
     */
    Window::Specification specification;
};

inline const GLFWwindow* const Window::GetNativeHandle() const noexcept
{
    return window;
}

inline const Window::Specification& Window::GetSpecification() const noexcept
{
    return specification;
}

inline bool Window::ShouldClose() const noexcept
{
    return glfwWindowShouldClose(window);
}

#endif // !GUARD_WINDOW_H