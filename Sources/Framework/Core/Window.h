#ifndef GUARD_WINDOW_H
#define GUARD_WINDOW_H

#include <GLFW/glfw3.h>

#include "WindowSpecification.h"

class Window
{
public:
    explicit Window(const WindowSpecification& specification_) noexcept;
    ~Window() noexcept;

    [[nodiscard]]
    inline const GLFWwindow* const GetNativeHandle() const noexcept;
    [[nodiscard]]
    inline const WindowSpecification& GetSpecification() const noexcept;
    [[nodiscard]]
    inline bool ShouldClose() const noexcept;

private:
    GLFWwindow* window;
    WindowSpecification specification;
};

inline const GLFWwindow* const Window::GetNativeHandle() const noexcept
{
    return window;
}

inline const WindowSpecification& Window::GetSpecification() const noexcept
{
    return specification;
}

inline bool Window::ShouldClose() const noexcept
{
    return glfwWindowShouldClose(window);
}

#endif // !GUARD_WINDOW_H