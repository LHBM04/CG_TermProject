#include "Scene.h"

#include "Objects.h"
#include "Rendering.h"
#include "Logger.h"

Scene::Scene() noexcept
{

}

Scene::~Scene() noexcept
{

}

void Scene::Enter() noexcept
{
	OnEnter();
}

void Scene::Update() noexcept
{
    for (auto& entity : objects)
    {
        entity->Update();
        entity->LateUpdate();
    }

	OnUpdate();
}

void Scene::Render() noexcept
{
    OnRender();
}

void Scene::Exit() noexcept
{
	OnExit();
}

Object* Scene::Emplace() noexcept
{
    return nullptr;
}

void Scene::Remove(Object entity) noexcept
{
  
}

void SceneManager::AddScene(const String& name_, Unique<Scene> scene_) noexcept
{
    if (scenes.contains(name_))
    {
        Logger::Error("Scene '{}' already exists.", name_);
        return;
    }

	scenes.emplace(name_,  std::move(scene_));
}

void SceneManager::RemoveScene(const String& name_) noexcept
{
    if (!scenes.contains(name_))
    {
        Logger::Error("Scene '{}' does not exist.", name_);
        return;
    }

	scenes.erase(name_);
}

void SceneManager::LoadScene(const String& name_) noexcept
{
	if (!scenes.contains(name_))
	{
        Logger::Error("Scene '{}' does not exist.", name_);
        return;
	}

    if (activeScene)
    {
        UnloadScene();
    }

    activeScene = scenes[name_].get();
    activeScene->Enter();
}

void SceneManager::UnloadScene() noexcept
{
    if (!activeScene)
    {
        Logger::Error("No active scene to unload.");
        return;
    }

    activeScene->Exit();
    activeScene = nullptr;
}

Map<String, Unique<Scene>> SceneManager::scenes;

Scene* SceneManager::activeScene = nullptr;