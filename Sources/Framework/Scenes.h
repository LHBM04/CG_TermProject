#pragma once

#include "Common.h"
#include "Objects.h"

class Shader;
class Texture;
class Mesh;

/**
 * @class Scene
 *
 * @brief 씬을 정의합니다.
 */
class Scene
{
public:
    /**
     * @brief 소멸자.
     */
    virtual ~Scene() noexcept;

    /**
     * @brief 해당 씬에 입장합니다.
     */
    void Enter() noexcept;

    /**
     * @brief 해당 씬을 매 프레임마다 갱신합니다.
     */
    void Update() noexcept;

    /**
     * @brief 해당 씬을 고정 프레임마다 갱신합니다.
     */
    void FixedUpdate() noexcept;

    /**
     * @brief 해당 씬을 렌더링합니다.
     */
    void Render() noexcept;

    /**
     * @brief 해당 씬의 UI를 렌더링합니다.
     */
    void RenderUI() noexcept;

    /**
     * @brief 해당 씬에서 퇴장합니다.
     */
    void Exit() noexcept;

protected:
    Object* AddGameObject(std::string_view name_, std::string_view tag_) noexcept;
    Object* AddUIObject(std::string_view name_, std::string_view tag_) noexcept;

    /**
     * @brief 씬에서 엔티티를 제거합니다.
     *
     * @param entity 제거할 엔티티
     */
    void Remove(Object entity) noexcept;

    /**
     * @brief 해당 씬에 입장할 때 호출됩니다.
     */
    virtual void OnEnter() noexcept
    {
    }

    /**
     * @brief 해당 씬을 매 프레임마다 갱신할 때 호출됩니다.
     */
    virtual void OnUpdate() noexcept
    {
    }

    /**
     * @brief 해당 씬을 고정 프레임마다 갱신할 때 호출됩니다.
     */
    virtual void OnFixedUpdate() noexcept
    {
    }

    /**
     * @brief 해당 씬의 게임 오브젝트를 렌더링할 때 호출됩니다.
     */
    virtual void OnRender() noexcept
    {
    }

    /**
     * @brief 해당 씬의 UI 오브젝트를 렌더링할 때 호출됩니다.
     */
    virtual void OnRenderUI() noexcept
    {
    }

    /**
     * @brief 해당 씬에서 퇴장할 때 호출됩니다.
     */
    virtual void OnExit() noexcept
    {
    }

private:
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Object>> uiObjects;
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
    static void AddScene(std::string_view name_, std::unique_ptr<Scene> scene_) noexcept;

    static void Initialize() noexcept;
    static void Update() noexcept;
    static void Render() noexcept;

    /**
     * @brief 씬을 제거합니다.
     *
     * @param name_ 제거할 씬의 이름.
     */
    static void RemoveScene(std::string_view name_) noexcept;

    /**
     * @brief 씬을 로드합니다.
     *
     * @param name_ 로드할 씬의 이름.
     */
    static void LoadScene(std::string_view name_) noexcept;

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
        return currentScene;
    }

private:
    /**
     * @brief 씬들을 저장하는 맵.
     */
    static std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;

    /**
     * @brief 현재 로드된 씬.
     */
    static Scene* currentScene;

    /**
     * @brief 다음에 로드할 씬.
     */
    static Scene* nextScene;

    static Shader*  loadingShader;
    static Mesh*    loadingMesh;
    static Texture* loadingTex;
    static float    texAlpha;
    static float    loadingAngle;
};