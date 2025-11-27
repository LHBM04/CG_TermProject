#include "Window.h"

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

Window::Window(const Window::Specification& specification_) noexcept
	: specification(specification_)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#if defined(_DEBUG)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    if (specification.isFullScreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        if (!monitor)
        {
            return;
        }

        const GLFWvidmode* const mode = glfwGetVideoMode(monitor);

        if (mode)
        {
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        }

        handle = glfwCreateWindow(mode->width, mode->height, specification.title, monitor, nullptr);
    }
    else
    {
        glfwWindowHint(GLFW_DECORATED, specification.isBorderless);
        glfwWindowHint(GLFW_RESIZABLE, specification.isResizable);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

         handle = glfwCreateWindow(specification.width, specification.height, specification.title, nullptr, nullptr);
    }

    if (!handle)
    {
        return;
    }

    if (specification.isVSync)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }
}

Window::~Window()
{
    if (handle)
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }
}