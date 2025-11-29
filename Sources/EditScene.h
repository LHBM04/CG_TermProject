#pragma once

#include "Framework/Rendering.h"
#include "Framework/Scene.h"

class EditScene : public Scene
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
    /**
     * @brief 메인 카메라.
     */
    Camera* mainCamera;
};