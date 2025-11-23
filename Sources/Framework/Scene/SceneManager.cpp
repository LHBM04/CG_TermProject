#include "SceneManager.h"

#include "Scene.h"

void SceneManager::AddScene(std::string_view name_, std::unique_ptr<Scene> scene_) noexcept
{
	if (scenes.contains(name_.data()))
	{
        return;
	}

	scenes.insert(std::make_pair(name_, std::move(scene_)));
}

void SceneManager::LoadScene(std::string_view name_) noexcept
{
	if (activeScene)
	{
		activeScene->Exit();
        lastScenes.push(activeScene);
    }

	activeScene = scenes[name_.data()].get();
    activeScene->Enter();
}

void SceneManager::UnloadScene() noexcept
{
    if (lastScenes.empty())
	{
		return;
    }

	activeScene->Exit();
    activeScene = lastScenes.top();
    lastScenes.pop();
    activeScene->Enter();
}

void SceneManager::Update() noexcept
{
	if (activeScene)
	{
		activeScene->Update();
	}
}

void SceneManager::Render() noexcept
{
	if (activeScene)
	{
		activeScene->Render();
	}
}

std::unordered_map<std::string, std::unique_ptr<Scene>> SceneManager::scenes{};

std::stack<Scene*> SceneManager::lastScenes{};

Scene* SceneManager::activeScene{nullptr};