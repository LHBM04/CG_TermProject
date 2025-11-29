#pragma once

#include "Common.h"

class Camera;
class Object;

/**
 * @class Scene
 * 
 * @brief 씬을 정의합니다.
 */
class Scene
{
public:
    /**
     * @brief 생성자.
     */
    Scene() noexcept;

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
     * @brief 고정 프레임마다 호출됩니다.
     */
    void FixedUpdate() noexcept;

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
     * @brief 해당 씬에 엔티티를 배치합니다.
     * 
     * @return 배치된 엔티티
     */
    Object* Emplace() noexcept;

    /**
     * @brief 씬에서 엔티티를 제거합니다.
     * 
     * @param entity 제거할 엔티티
     */
    void Remove(Object entity) noexcept;

    /**
     * @brief
     */
    virtual void OnEnter() noexcept = 0;

    /**
     * @brief
     */
    virtual void OnUpdate() noexcept = 0;

    /**
     * @brief 
     */
    virtual void OnFixedUpdate() noexcept = 0;

    /**
     * @brief
     */
    virtual void OnRender() noexcept = 0;

    /**
     * @brief
     */
    virtual void OnExit() noexcept = 0;

private:
    /**
     * @brief 모든 엔티티와 컴포넌트 데이터를 저장하는 저장소.
     */
    List<Unique<Object>> objects;
};

/**
 * @class SceneManager
 * 
 * @brief 
 */
class SceneManager final
{
    STATIC_CLASS(SceneManager)

public:
    /**
     * @brief 씬을 추가합니다.
     * 
     * @param name_  추가할 씬의 이름
     * @param scene_ 추가할 씬
     */
    static void AddScene(const String& name_, Unique<Scene> scene_) noexcept;

    /**
     * @brief 씬을 제거합니다.
     * 
     * @param name_ 제거할 씬의 이름.
     */
    static void RemoveScene(const String& name_) noexcept;

    /**
     * @brief 씬을 로드합니다.
     * 
     * @param name_ 로드할 씬의 이름.
     */
    static void LoadScene(const String& name_) noexcept;

    /**
     * @brief 씬을 언로드합니다.
     */
    static void UnloadScene() noexcept;

    /**
     * @brief 현재 로드된 씬을 반환합니다.
     * 
     * @return Scene* 현재 로드된 씬
     */
    [[nodiscard]]
    inline static Scene* GetActiveScene() noexcept
    {
        return activeScene;
    }

private:
    /**
     * @brief 씬들을 저장하는 맵.
     */
    static Map<String, Unique<Scene>> scenes;

    /**
     * @brief 현재 로드된 씬.
     */
    static Scene* activeScene;
};