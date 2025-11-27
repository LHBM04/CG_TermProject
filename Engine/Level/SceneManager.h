#pragma once

#include <cstddef>
#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>

class Application;
class Scene;

/**
 * @class SceneManager
 *
 * @brief 월드 관리를 정의합니다.
 */
class SceneManager final
{
    friend class Application;
    friend class Scene;

public:
#pragma region Deleted Functions
    SceneManager()  = delete;
    ~SceneManager() = delete;

    SceneManager(const SceneManager&) = delete;
    SceneManager(SceneManager&&)      = delete;

    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&)      = delete;

    void* operator new(std::size_t)   = delete;
    void* operator new[](std::size_t) = delete;
    void  operator delete(void*)      = delete;
    void  operator delete[](void*)    = delete;
#pragma endregion

    /**
     * @brief 현재 열려 있는 Scene을 반환합니다.
     *
     * @return Scene* 현재 열려 있는 Scene.
     */
    [[nodiscard]]
    static inline Scene* GetActiveScene() noexcept;

    /**
     * @brief 새로운 Scene을 추가합니다.
     *
     * @param name_  추가할 Scene의 이름
     * @param scene_ 추가할 Scene의 인스턴스
     */
    static void AddScene(std::string_view name_, std::unique_ptr<Scene> scene_) noexcept;

    /**
     * @brief 지정한 Scene을 불러옵니다.
     *
     * @param name_ 불러올 Scene의 이름.
     */
    static void LoadScene(std::string_view name_) noexcept;

    /**
     * @brief 현재 열려 있는 Scene을 닫고, 이전 Scene으로 돌아갑니다.
     */
    static void UnloadScene() noexcept;

private:
    /**
     * @brief 현재 열려 있는 Scene을 업데이트합니다.
     */
    static void Update() noexcept;

    /**
     * @brief 현재 열려 있는 Scene을 렌더링합니다.
     */
    static void Render() noexcept;

    /**
     * @brief 애플리케이션 내 모든 Scene.
     */
    static std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;

    /**
     * @brief 이전에 열려 있었던 Scene들.
     */
    static std::stack<Scene*> lastScenes;

    /**
     * @brief 현재 열려 있는 Scene.
     */
    static Scene* activeScene;
};

inline Scene* SceneManager::GetActiveScene() noexcept
{
    return activeScene;
}