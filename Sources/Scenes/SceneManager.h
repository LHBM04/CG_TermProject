#pragma once

#include "../PCH.h"

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
    /**
     * @brief 현재 열려 있는 Scene을 반환합니다.
     *
     * @return Scene* 현재 열려 있는 Scene.
     */
    [[nodiscard]]
    static inline Scene* GetActiveScene() noexcept;

    /**
     * @brief 지정한 Scene을 불러옵니다.
	 * 
     * @param name_ 불러올 Scene의 이름.
     */
    static void LoadScene(std::string_view name_) noexcept;

	template <typename T>
        requires std::is_base_of_v<Scene, T>
	static void AddScene() noexcept;

	/**
     * @brief 현재 열려 있는 Scene을 닫고, 이전 Scene으로 돌아갑니다.
	 */
	static void UnloadScene() noexcept;

private:
	SceneManager()  = delete;
	~SceneManager() = delete;

	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;

    SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	void* operator new(size_t) = delete;
	void* operator new[](size_t) = delete;
	void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

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
	static std::vector<std::shared_ptr<Scene>> scenes;

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

template <typename T>
    requires std::is_base_of_v<Scene, T>
void SceneManager::AddScene() noexcept
{
    SceneManager::scenes.push_back(std::make_shared<T>());
}