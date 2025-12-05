#include "Framework/Application.h"
#include "Framework/Scenes.h"

#include "EditScene.h"

int main(int, char**)
{
    Application::Specification spec;
    spec.name        = "Labyrinth Editor";
    spec.width       = 1280;
    spec.height      = 720;
    spec.screenMode  = Application::WindowMode::Windowed;
    spec.isVSync = true;

    if (!Application::Initialize(spec))
    {
        return -1;
    }

    SceneManager::AddScene("EditScene", std::make_unique<EditScene>());
    SceneManager::LoadScene("EditScene");

    return Application::Run();
}