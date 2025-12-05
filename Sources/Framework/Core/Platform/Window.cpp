#include "Window.hpp"

#include <GLFW/glfw3.h>

#include "../Debug/Debugger.hpp"
#include "../Debug/Logger.hpp"

Window::Window(const Window::Specification& specification_) noexcept
    : specification(specification_)
    , native(nullptr)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, specification_.contextVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, specification_.contextVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (specification.fullscreen)
    {
        if (!specification.monitor)
        {
            Logger::Critical("Failed to get primary monitor for fullscreen window!");
            return;
        }
        else
        {
            const GLFWvidmode* const mode = glfwGetVideoMode(specification.monitor);

            if (mode)
            {
                glfwWindowHint(GLFW_RED_BITS, mode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            }
        }
    }
    else
    {
        glfwWindowHint(GLFW_DECORATED, specification.decorated);
        glfwWindowHint(GLFW_RESIZABLE, specification.resizable);
    }

    native = glfwCreateWindow(
        specification.width,
        specification.height,
        specification.title.c_str(),
        specification.monitor,
        nullptr);

    if (!native)
    {
        Logger::Critical("Failed to create GLFW window!");
        return;
    }

    glfwMakeContextCurrent(native);
    glfwSetWindowPos(native, specification.x, specification.y);
    glfwSwapInterval(specification.vSync ? 1 : 0);

    glfwSetWindowUserPointer(native, &specification);
}

Window::~Window() noexcept
{
    if (native)
    {
        glfwDestroyWindow(native);
        native = nullptr;
    }
}

bool Window::IsFullscreen() const noexcept
{
    return specification.fullscreen;
}

void Window::SetFullscreen(bool enabled_) noexcept
{
    if (specification.fullscreen == enabled_)
    {
        return;
    }

    specification.fullscreen = enabled_;

    GLFWmonitor* monitor = enabled_ ? glfwGetPrimaryMonitor() : nullptr;

    if (enabled_)
    {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(native, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(native, nullptr, specification.x, specification.y, specification.width, specification.height, 0);

        glfwSetWindowAttrib(native, GLFW_DECORATED, specification.decorated);
        glfwSetWindowAttrib(native, GLFW_RESIZABLE, specification.resizable);
    }
}

bool Window::IsDecorated() const noexcept
{
    return specification.decorated;
}

void Window::SetDecorated(bool enabled_) noexcept
{
    specification.decorated = enabled_;
    
    if (!specification.fullscreen)
    {
        glfwSetWindowAttrib(native, GLFW_DECORATED, enabled_ ? GLFW_TRUE : GLFW_FALSE);
    }
}

bool Window::IsResizable() const noexcept
{
    return specification.resizable;
}

void Window::SetResizable(bool enabled_) noexcept
{
    specification.resizable = enabled_;
 
    if (!specification.fullscreen)
    {
        glfwSetWindowAttrib(native, GLFW_RESIZABLE, enabled_ ? GLFW_TRUE : GLFW_FALSE);
    }
}

bool Window::IsVSync() const noexcept
{
    return specification.vSync;
}

void Window::SetVSync(bool enabled_) noexcept
{
    specification.vSync = enabled_;
    glfwSwapInterval(enabled_ ? 1 : 0);
}

GLFWwindow* const Window::GetNativeHandle() const noexcept
{
    return native;
}