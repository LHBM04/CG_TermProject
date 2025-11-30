#include "Application.h"

#include "Debug.h"
#include "Input.h"
#include "Scenes.h"
#include "Time.h"

bool Application::Initialize(const Specification& specification_) noexcept
{
    specification = specification_;

	Logger::Initialize();

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

	switch (specification.screenMode)
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
    glfwSwapInterval(specification.sholudVSync ? 1 : 0);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        SPDLOG_CRITICAL("Failed to initialize GLAD");
        return false;
    }

    InputManager::Initialize(window); 
    TimeManager::Initialize();

    return true;
}

int Application::Run() noexcept
{
    while (!glfwWindowShouldClose(const_cast<GLFWwindow*>(window)))
    {
        glfwPollEvents();

        InputManager::Update();
        TimeManager::Update();

        Update();
        Render();
    }

    return 0;
}

void Application::Quit() noexcept
{
    glfwSetWindowShouldClose(const_cast<GLFWwindow*>(window), GLFW_TRUE);
}

void Application::Update() noexcept
{
    Scene* const activeScene = SceneManager::GetActiveScene();
    if (!activeScene)
    {
        return;
    }

    static float fixedUpdateTime = 0.0f;
    fixedUpdateTime += TimeManager::GetDeltaTime();

    if (const float fixedDeltaTime = TimeManager::GetUnscaledDeltaTime(); fixedDeltaTime > 0.0f)
    {
        while (fixedUpdateTime >= fixedDeltaTime)
        {
            fixedUpdateTime -= fixedDeltaTime;
        }

        fixedUpdateTime = 0.0f;
    }

    activeScene->Update();
}

void Application::Render() noexcept
{
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Scene* const activeScene = SceneManager::GetActiveScene();
    if (!activeScene)
    {
        return;
    }

    activeScene->Render();

    glfwSwapBuffers(const_cast<GLFWwindow*>(window));
}

Application::Specification Application::specification;

GLFWwindow* Application::window = nullptr;

glm::fvec3 Application::clearColor = glm::fvec3(0.1f, 0.1f, 0.1f);