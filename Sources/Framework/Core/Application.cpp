#include "Application.hpp"

#include <stdexcept>
#include <exception>

#include <windows.h>
#include <windowsx.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Debug/Debugger.hpp"
#include "Debug/Logger.hpp"

namespace
{
    /**
     * @brief 애플리케이션의 사양.
     */
    Application::Specification specification;

    /**
     * @brief GLFW 윈도우 핸들.
     */
    GLFWwindow* window;
} // namespace

bool Application::Initialize(const Specification& specification_) noexcept
{
    specification = specification_;

    if (!glfwInit())
    {
        Logger::Critical("Failed to initialize GLFW.");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(DEBUG) || defined(_DEBUG)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    switch (specification.windowMode)
    {
        case Application::WindowMode::Windowed:
        {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            window = glfwCreateWindow(
                    specification.width, specification.height, specification.name.c_str(), nullptr, nullptr);
            break;
        }
        case Application::WindowMode::FullScreen:
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            if (!monitor)
            {
                return false;
            }
            const GLFWvidmode* const mode = glfwGetVideoMode(monitor);
            if (mode)
            {
                glfwWindowHint(GLFW_RED_BITS, mode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            }
            window = glfwCreateWindow(mode->width, mode->height, specification.name.c_str(), monitor, nullptr);
            break;
        }
        case Application::WindowMode::Borderless:
        {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            window = glfwCreateWindow(
                    specification.width, specification.height, specification.name.c_str(), nullptr, nullptr);
            break;
        }
        default:
        {
            break;
        }
    }
    if (!window)
    {
        Logger::Critical("Failed to create fullscreen GLFW window");
        return false;
    }

    glfwMakeContextCurrent(const_cast<GLFWwindow*>(window));
    glfwSwapInterval(specification.vSync ? 1 : 0);
    
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        Logger::Critical("Failed to initialize GLAD");
        return false;
    }
    
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

int Application::Run() noexcept
{
    while (!glfwWindowShouldClose(const_cast<GLFWwindow*>(window)))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(const_cast<GLFWwindow*>(window));
    }
    return 0;
}

void Application::Quit(const int exitCode_) noexcept
{
    glfwSetWindowShouldClose(const_cast<GLFWwindow*>(window), GLFW_TRUE);

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
    ExitProcess(static_cast<UINT>(exitCode_));
}