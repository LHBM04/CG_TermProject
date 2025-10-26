#include "Window.h"

Window::Window(const WindowSpecification &specification_) noexcept
        : specification(specification_)
{
    if (!glfwInit())
    {
        // TODO: 로그 기록
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, specification.decorated ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, specification.resizable ? GLFW_TRUE : GLFW_FALSE);

    int width = specification.width;
    int height = specification.height;
    GLFWmonitor *monitor = nullptr;

    if (specification.fullscreen)
    {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);
        width = videoMode->width;
        height = videoMode->height;
    }

    window = glfwCreateWindow(width, height, specification.title, monitor, nullptr);
    if (!window)
    {
        // TODO: 로그 기록
        glfwTerminate();
        return;
    }

    glfwSetWindowPos(window, specification.x, specification.y);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(specification.vsync ? 1 : 0);
}

Window::~Window() noexcept
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
}