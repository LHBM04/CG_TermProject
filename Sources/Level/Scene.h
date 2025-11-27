#pragma once

#include <memory>
#include <vector>

#include <entt/entt.hpp>

class Camera;
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
     * @brief 카메라를 추가합니다.
     *
     * @param camera_ 추가할 카메라
     */
    void Emplace(std::unique_ptr<Camera> camera_) noexcept;

    /**
     * @brief 객체를 추가합니다.
     */
    void Emplace() noexcept;

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
     * @brief 해당 Scene의 카메라들.
     */
    std::vector<std::unique_ptr<Camera>> cameras;

    /**
     * @brief 해당 씬의 엔티티-컴포넌트 시스템 레지스트리.
     */
    entt::registry entities;

    /**
     * @brief 해당 씬의 인터페이스용 엔티티-컴포넌트 시스템 레지스트리.
     */
    entt::registry interfaces;
};