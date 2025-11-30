#pragma once

#include "../Framework/Application.h"
#include "../Framework/Input.h"
#include "../Framework/Scenes.h"

/**
 * @class TitleScene
 *
 * @brief 타이틀 씬을 정의합니다.
 */
class TitleScene final : public Scene
{
public:
    virtual void OnUpdate() noexcept override
    {
        if (InputManager::IsKeyPressed(Keyboard::Enter))
        {
            Application::Quit();
        }
    }
};