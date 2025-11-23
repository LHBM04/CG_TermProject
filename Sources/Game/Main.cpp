#include <memory>

#include "../Framework/Core/Application.h"
#include "../Framework/Scene/SceneManager.h"

#include "Scenes/TestScene.h"

int main(int, char**) 
{
    Application::Specification specification = {};
    specification.name                       = "My Application";
    specification.width                      = 1280;
    specification.height                     = 720;
    specification.screenMode                 = Application::WindowMode::Windowed;
    specification.sholudVSync                = true;

    if (!Application::Initialize(specification))
    {
        return -1;
    }

    SceneManager::AddScene("Test Scene 1", std::make_unique<TestScene>());
    SceneManager::AddScene("Test Scene 2", std::make_unique<TestScene>());
    SceneManager::LoadScene("Test Scene 1");

    return Application::Run();
}