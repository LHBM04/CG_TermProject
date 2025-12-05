#include "Windowing.h"

#include <memory>
#include <vector>

#include <windows.h>
#include <windowsx.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace
{
    /**
     * @brief 윈도우.
     */
    std::vector<std::unique_ptr<Windowing::Window>> windows;
}

Windowing::Window::Window(const Windowing::Specification& specification_) noexcept
{
    if (windows.empty())
    {
        if (!glfwInit())
        {

        }
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(DEBUG) || defined(_DEBUG)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    GLFWmonitor* monitor = nullptr;
    if (spec.mode == WindowMode::FullScreen)
    {
        monitor                 = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        spec.width              = mode->width;
        spec.height             = mode->height;
    }
    else if (spec.mode == WindowMode::Borderless)
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    }

    GLFWwindow* newWindow = glfwCreateWindow(spec.width, spec.height, spec.title.c_str(), monitor, spec.shareContext);

    if (!nativeWindow)
    {
        Logger::Critical("Failed to create GLFW window!");
        // 생성 실패 시에도 카운트 정합성을 위해 terminate 체크 필요할 수 있음
        if (windowCount == 0)
            glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
}