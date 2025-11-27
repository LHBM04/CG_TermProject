#include "../Engine/Core/Application.h"

static void Start();

static void Update();

static void Render();

static void Shutdown();

int main(int, char**)
{
    Application::Specification specification;
    specification.name        = "Labyrinth Editor";
    specification.width       = 1280;
    specification.height      = 720;
    specification.screenMode  = Application::WindowMode::Windowed;
    specification.sholudVSync = true;

    if (!Application::Initialize(specification))
    {
        return -1;
    }

    return Application::Run();
}