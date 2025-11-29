#include "TitleScene.h"

#include "Framework/Application.h"
#include "Framework/Input.h"
#include "Framework/Logger.h"

void TitleScene::OnEnter() noexcept
{
    Logger::Info("Entered Title Scene.");
}

void TitleScene::OnUpdate() noexcept
{
    if (Input::IsKeyPressed(Keyboard::Enter))
    {
        Application::Quit();
    }
}

void TitleScene::OnFixedUpdate() noexcept
{
}

void TitleScene::OnRender() noexcept
{

}

void TitleScene::OnExit() noexcept
{
    Logger::Info("Exited Title Scene.");
}