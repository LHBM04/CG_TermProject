#include "Scenes.h"

#include "Debug.h"
#include "Objects.h"
#include "Rendering.h"

Scene::~Scene() noexcept
{
}

void Scene::Enter() noexcept
{
    OnEnter();
}

void Scene::Update() noexcept
{
    for (const std::unique_ptr<Object>& entity : objects)
    {
        entity->Update();
    }

    OnUpdate();
}

void Scene::FixedUpdate() noexcept
{
    for (const std::unique_ptr<Object>& object : objects)
    {
        object->FixedUpdate();
    }

    std::erase_if(objects, [](const std::unique_ptr<Object>& object) { return object->IsDestroyed(); });

    OnFixedUpdate();
}

void Scene::Render() noexcept
{
    std::vector<Camera*> cameras;

    for (const std::unique_ptr<Object>& object : objects)
    {
        if (!object->IsEnabled())
        {
            continue;
        }

        if (Camera* camera = object->GetComponent<Camera>())
        {
            if (camera->IsEnabled())
            {
                cameras.push_back(camera);
            }
        }
    }

    if (cameras.empty())
    {
        Logger::Warn("No active camera found in the scene. Skipping rendering.");
        return;
    }

    for (Camera* camera : cameras)
    {
        camera->Ready();

        for (const std::unique_ptr<Object>& object : objects)
        {
            if (object->IsEnabled())
            {
                object->Render();
            }
        }
    }

    OnRender();
}

void Scene::RenderUI() noexcept
{
    for (const std::unique_ptr<Object>& object : uiObjects)
    {
        if (!object->IsEnabled())
        {
            continue;
        }

        object->Render();
    }

    // 3. 씬 별 추가 커스텀 UI 로직 호출
    OnRenderUI();
}

void Scene::Exit() noexcept
{
    objects.clear();

    OnExit();
}

Object* Scene::AddGameObject(std::string_view name_, std::string_view tag_) noexcept
{
    return objects.emplace_back(std::make_unique<Object>(name_, tag_)).get();
}

Object* Scene::AddUIObject(std::string_view name_, std::string_view tag_) noexcept
{
    return uiObjects.emplace_back(std::make_unique<Object>(name_, tag_)).get();
}

void Scene::Remove(Object entity) noexcept
{
    entity.Destroy();
}

void SceneManager::AddScene(std::string_view name_, std::unique_ptr<Scene> scene_) noexcept
{
    if (scenes.contains(name_.data()))
    {
        Logger::Error("Scene '{}' already exists.", name_);
        return;
    }

    scenes.emplace(name_.data(), std::move(scene_));
}

void SceneManager::RemoveScene(std::string_view name_) noexcept
{
    if (!scenes.contains(name_.data()))
    {
        Logger::Error("Scene '{}' does not exist.", name_);
        return;
    }

    scenes.erase(name_.data());
}

void SceneManager::LoadScene(std::string_view name_) noexcept
{
    if (!scenes.contains(name_.data()))
    {
        Logger::Error("Scene '{}' does not exist.", name_);
        return;
    }

    if (activeScene)
    {
        UnloadScene();
    }

    activeScene = scenes[name_.data()].get();
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

std::unordered_map<std::string, std::unique_ptr<Scene>> SceneManager::scenes;

Scene* SceneManager::activeScene = nullptr;