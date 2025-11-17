#include "Application.h"

#include "Input.h"
#include "Time.h"

#include "../Scenes/Scene.h"
#include "../Scenes/SceneManager.h"

bool Application::Initialize(const Application::Specification& specification_) noexcept
{
    specification = specification_;

    SPDLOG_TRACE("TEST");
	if (glfwInit() == GLFW_FALSE)
	{
        SPDLOG_CRITICAL("Failed to initialize GLFW");
		return false;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#if defined(_DEBUG)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    switch (specification.screenMode)
    {
        case WindowMode::Windowed:
        {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

            window = glfwCreateWindow(specification.width,
                                      specification.height,
                                      specification.name.c_str(),
                                      nullptr, nullptr);

            break;
        }
        case WindowMode::FullScreen:
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

            window = glfwCreateWindow(mode->width,
                                      mode->height,
                                      specification.name.c_str(),
                                      monitor, 
                                      nullptr);

            break;
        }
        case WindowMode::Borderless:
        {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

            window = glfwCreateWindow(specification.width,
                                      specification.height,
                                      specification.name.c_str(),
                                      nullptr, nullptr);

            break;
        }
        default:
        {
            break;
        }
    }

    if (!window)
    {
        SPDLOG_CRITICAL("Failed to create fullscreen GLFW window");
        return false;
    }

    glfwMakeContextCurrent(const_cast<GLFWwindow*>(window));
    glfwSwapInterval(specification.sholudVSync ? 1 : 0);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        SPDLOG_CRITICAL("Failed to initialize GLAD");
        return false;
    }

#ifdef _DEBUG
    int flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(
            [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
            {
                SPDLOG_ERROR("GL DEBUG MESSAGE: {}", message);
            },
            nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

	return true;
}

int Application::Run() noexcept
{
    Time::Initialize();
    Input::Initialize(const_cast<GLFWwindow*>(window));
    
    while (!glfwWindowShouldClose(const_cast<GLFWwindow*>(window)))
    {
        Time::Update();
        
        static float fixedUpdateTime = 0.0f;
        fixedUpdateTime += Time::GetDeltaTime();

        if (const float fixedDeltaTime = Time::GetUnscaledDeltaTime(); fixedDeltaTime > 0.0f)
        {
            while (fixedUpdateTime >= fixedDeltaTime)
            {
                fixedUpdateTime -= fixedDeltaTime;
            }

            fixedUpdateTime = 0.0f;
        }

        SceneManager::Update();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SceneManager::Render();

        Input::Update();

        glfwPollEvents();
        glfwSwapBuffers(const_cast<GLFWwindow*>(window));
    }

    return 0;
}

void Application::Quit() noexcept
{
    glfwSetWindowShouldClose(const_cast<GLFWwindow*>(window), GLFW_TRUE);
}

Application::Specification Application::specification = {};

volatile GLFWwindow* Application::window = nullptr;