#include "Scenes.h"

#include "Objects.h"
#include "Rendering.h"
#include "Debug.h"

Scene::Scene() noexcept
{
    Object* camera = AddObject("Main Camera", "Camera");
    camera->AddComponent<Camera>();
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
    // [1단계] 현재 씬에서 활성화된 모든 카메라를 수집합니다.
    std::vector<Camera*> cameras;

    for (const std::unique_ptr<Object>& object : objects)
    {
        // 비활성화된 오브젝트는 건너뜀
        if (!object->IsEnabled())
        {
            continue;
        }

        // type_index로 최적화된 GetComponent를 사용하여 빠르게 검사
        if (Camera* camera = object->GetComponent<Camera>())
        {
            // 카메라도 켜져 있어야 함
            if (camera->IsEnabled())
            {
                cameras.push_back(camera);
            }
        }
    }

    // 활성화된 카메라가 없으면 그릴 필요가 없음
    if (cameras.empty())
    {
        Logger::Warn("No active camera found in the scene. Skipping rendering.");
        return;
    }

    // [2단계] 수집된 카메라를 기준으로 렌더링을 수행합니다.
    for (Camera* camera : cameras)
    {
        // 셰이더 설정 및 뷰포트/Clear 처리
        camera->Ready();

        // 모든 오브젝트 렌더링
        for (const std::unique_ptr<Object>& object : objects)
        {
            if (object->IsEnabled())
            {
                object->Render();
            }
        }
    }

    // UI 렌더링 등 후처리
    OnRender();
}

void Scene::Exit() noexcept
{
	OnExit();
}

Object* Scene::AddObject(std::string_view name_, std::string_view tag_) noexcept
{
    return objects.emplace_back(std::make_unique<Object>(name_, tag_)).get();
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