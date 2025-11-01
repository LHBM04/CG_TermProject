#include "Application.h"

bool Application::Initialize(const Application::Specification& specification_)
{
    specification = specification_;
    {
        Window::Specification spec{};
        spec.x          = 100;
        spec.y          = 100;
        spec.width      = 800;
        spec.height     = 600;
        spec.title      = "My Application";
        spec.resizable  = true;
        spec.fullscreen = false;
        spec.vsync      = true;
        spec.decorated  = true;

        window = std::make_unique<Window>(spec);
    }
    {
        Graphics::Specification spec{};
        spec.x      = 0;
        spec.y      = 0;
        spec.width  = 800;
        spec.height = 600;

        graphics = std::make_unique<Graphics>(spec);
    }

    return true;
}

void Application::Run()
{
    while (!window->ShouldClose())
    {
        window->Update();

        graphics->PreRender();
        graphics->PostRender();
    }
}

Application::Specification Application::specification{};

std::unique_ptr<Window> Application::window = nullptr;

std::unique_ptr<Graphics> Application::graphics = nullptr;