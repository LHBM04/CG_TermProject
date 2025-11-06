#include "Application.h"

#include "Input.h"
#include "Time.h"

int Application::Run(const Application::Specification& specification_)
{
    specification = specification_;

    if (!InitWindow())
    {
        spdlog::critical("Failed to initialize window.");
        return -1;
    }

    if (!InitGraphics())
    {
        spdlog::critical("Failed to initialize graphics.");
        return -1;
    }

    glfwSetKeyCallback(window, [](GLFWwindow*, int, int, int, int) noexcept { Input::OnKeyInteracted(); });
    glfwSetMouseButtonCallback(window, [](GLFWwindow*, int, int, int) { Input::OnMouseButton(); });

    while (glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    if (window != nullptr)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    return 0;
}

void Application::Quit() noexcept
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);

    
}

Application::Specification Application::specification{};

GLFWwindow* Application::window = nullptr;