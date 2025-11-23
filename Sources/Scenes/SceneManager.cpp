#include "SceneManager.h"

#include "Scene.h"

void SceneManager::LoadScene(std::string_view name_) noexcept
{
	if (activeScene)
	{
		activeScene->Exit();
        lastScenes.push(activeScene);
    }

	for (const auto& scene : scenes)
	{
		if (scene->name == name_)
		{
            activeScene = scene.get();
			activeScene->Enter();
			return;
        }
	}
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

std::vector<std::shared_ptr<Scene>> SceneManager::scenes{};

std::stack<Scene*> SceneManager::lastScenes{};

Scene* SceneManager::activeScene{nullptr};