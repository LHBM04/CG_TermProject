#pragma once

#include "../PCH.h"

class SceneManager;

/**
 * @class Scene
 * 
 * @brief 애플리케이션 내 월드를 정의합니다.
 */
class Scene
{
    friend class SceneManager;

public:
    /**
     * @brief 생성자.
     * 
     * @param name_ 해당 월드의 이름.
     */
    Scene(std::string_view name_) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Scene() noexcept;

    /**
     * @brief 월드에 진입할 때 호출됩니다.
     */
    void Enter() noexcept;

    /**
     * @brief 매 프레임마다 호출됩니다.
     */
    void Update() noexcept;

    /**
     * @brief 매 프레임마다 렌더링할 때 호출됩니다.
     */
    void Render() noexcept;
    
    /**
     * @brief 월드에서 나갈 때 호출됩니다.
     */
    void Exit() noexcept;

protected:
    /**
     * @brief 
     */
    virtual void OnEnter() noexcept;

    /**
     * @brief 
     */
    virtual void OnUpdate() noexcept;

    /**
     * @brief 
     */
    virtual void OnRender() noexcept;

    /**
     * @brief 
     */
    virtual void OnExit() noexcept;

private:
    /**
     * @brief 해당 월드의 이름.
     */
    std::string name;
};