#include "Core/Application.h"

#include "Scenes/SceneManager.h"
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

    SceneManager::AddScene<TestScene>();
    SceneManager::LoadScene("TestScene");

    return Application::Run();
}