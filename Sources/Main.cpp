#include "Framework/Application.h"

#include "Framework/Scene.h"

#include "TitleScene.h"
#include "GameScene.h"
#if defined(DEBUG) || defined(_DEBUG)
    #include "EditScene.h"
#endif

int main()
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

    SceneManager::AddScene("Title Scene", MakeUnique<TitleScene>());
    SceneManager::AddScene("Stage Scene", MakeUnique<GameScene>());
#if defined(DEBUG) || defined(_DEBUG)
    SceneManager::AddScene("Edit Scene", MakeUnique<EditScene>());
#endif

    SceneManager::LoadScene("Edit Scene");

    return Application::Run();
}