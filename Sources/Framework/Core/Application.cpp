#include "Application.h"

void Application::Initialize()
{
    WindowSpecification spec{};
    spec.x = 100;
    spec.y = 100;
    spec.width = 800;
    spec.height = 600;
    spec.title = "My Application";
    spec.resizable = true;
    spec.fullscreen = false;
    spec.vsync = true;
    spec.decorated = true;

    window = std::make_unique<Window>(spec);
    graphics = std::make_unique<Graphics>();
}

void Application::Run()
{
    while (!window->ShouldClose())
    {
        graphics->PreRender();
        graphics->PostRender();
    }
}

std::unique_ptr<Window> Application::window = nullptr;
std::unique_ptr<Graphics> Application::graphics = nullptr;