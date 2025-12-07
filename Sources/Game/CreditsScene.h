#pragma once

#include "Framework/Scenes.h"

class CreditsScene : public Scene
{
protected:
    virtual void OnEnter() noexcept override;
    virtual void OnUpdate() noexcept override;
};