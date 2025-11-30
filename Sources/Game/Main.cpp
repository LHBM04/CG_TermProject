#include "../Framework/Application.h"

int main(int, char**)
{
    Application::Specification spec;
    spec.name = "Labyrinth Application";
    spec.width = 1280;
    spec.height = 720;
    spec.screenMode = Application::WindowMode::Windowed;
    spec.sholudVSync = true;

    if (!Application::Initialize(spec))
    {
        return -1;
    }

    return Application::Run();
}