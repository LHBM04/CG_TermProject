#pragma once

#include "Framework/Scene.h"

class GameScene : public Scene
{
protected:
    /**
     * @brief
     */
    virtual void OnEnter() noexcept override;
    /**
     * @brief
     */
    virtual void OnUpdate() noexcept override;
    /**
     * @brief 
     */
    virtual void OnFixedUpdate() noexcept override;
    /**
     * @brief
     */
    virtual void OnRender() noexcept override;
    /**
     * @brief
     */
    virtual void OnExit() noexcept override;

private:
};