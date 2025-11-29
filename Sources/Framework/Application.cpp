#include "Application.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Input.h"
#include "Logger.h"
#include "Scene.h"
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
        case WindowMode::Windowed:
        {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

            window = glfwCreateWindow(
                    specification.width, specification.height, specification.name.c_str(), nullptr, nullptr);

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

            window = glfwCreateWindow(mode->width, mode->height, specification.name.c_str(), monitor, nullptr);

            break;
        }
        case WindowMode::Borderless:
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

    glfwSetKeyCallback(window, [](GLFWwindow*, i32 key_, i32 scancode_, i32 action_, i32 mods_) { 
        Input::OnKeyInteracted(key_, scancode_, action_, mods_); 
    });
    glfwSetMouseButtonCallback(window,[](GLFWwindow*, i32 button_, i32 action_, i32 mods_) {
        Input::OnMouseButtonInteracted(button_, action_, mods_);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow*, f64 x_, f64 y_) {
        Input::OnMouseMoved(x_, y_);
    });
    glfwSetScrollCallback(window, [](GLFWwindow*, f64 x_, f64 y_) {
        Input::OnMouseScrolled(x_, y_);
    });

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        SPDLOG_CRITICAL("Failed to initialize GLAD");
        return false;
    }

#if defined(DEBUG) || defined(_DEBUG)
    int flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback([](GLenum        source,
                                  GLenum        type,
                                  GLuint        id,
                                  GLenum        severity,
                                  GLsizei       length,
                                  const GLchar* message,
                                  const void*   userParam) { Logger::Error("GL DEBUG MESSAGE: {}", message); },
                               nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 키보드 제어 허용
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // 도킹 허용 (Docking 브랜치 사용 중이므로 필수)

    // 2. 스타일 설정
    ImGui::StyleColorsDark();

    // 3. 백엔드 초기화 (Platform: GLFW, Renderer: OpenGL3)
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    return true;
}

i32 Application::Run() noexcept
{
    while (!glfwWindowShouldClose(const_cast<GLFWwindow*>(window)))
    {
        glfwPollEvents();

        Time::Update();
        Input::Update();

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
    fixedUpdateTime += Time::GetDeltaTime();

    if (const float fixedDeltaTime = Time::GetUnscaledDeltaTime(); fixedDeltaTime > 0.0f)
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

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    activeScene->Render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(const_cast<GLFWwindow*>(window));
}

Application::Specification Application::specification;

GLFWwindow* Application::window = nullptr;

FVector3 Application::clearColor = FVector3(0.1f, 0.1f, 0.1f);