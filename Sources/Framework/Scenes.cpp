#include "Scenes.h"

#include "Application.h"
#include "Debug.h"
#include "Objects.h"
#include "Rendering.h"
#include "Time.h"

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

void SceneManager::Initialize() noexcept
{
    loadingShader = ResourceManager::LoadResource<Shader>("Assets/Shaders/UIObject");
    loadingMesh   = ResourceManager::LoadResource<Mesh>("Assets/Meshes/Rect.obj");
    loadingTex    = ResourceManager::LoadResource<Texture>("Assets/Textures/Loading.png");
}

void SceneManager::Update() noexcept
{
    if (nextScene)
    {
        texAlpha += TimeManager::GetUnscaledDeltaTime() * 2.0f;
        if (texAlpha >= 1.0f)
        {
            if (currentScene)
            {
                currentScene->Exit();
            }

            texAlpha     = 1.0f;
            currentScene = nextScene;
            currentScene->Enter();
            nextScene = nullptr;
        }
    }
    else
    {
        texAlpha -= TimeManager::GetUnscaledDeltaTime() * 2.0f;
        if (texAlpha <= 0.0f)
        {
            texAlpha = 0.0f;
        }
    }
}

void SceneManager::Render() noexcept
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadingShader->Use();

    float width  = (float)Application::GetWindowWidth();
    float height = (float)Application::GetWindowHeight();

    glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
    loadingShader->SetUniformMatrix4x4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model           = glm::translate(model, glm::vec3(width / 2.0f, height / 2.0f, 0.0f));
    model           = glm::scale(model, glm::vec3(width, height, 1.0f));

    loadingShader->SetUniformMatrix4x4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, loadingTex->GetTextureID());
    loadingShader->SetUniformInt("outTexture", 0);

    loadingShader->SetUniformVector4("color", glm::vec4(1.0f, 1.0f, 1.0f, texAlpha));

    loadingMesh->Draw();
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

    nextScene = scenes[name_.data()].get();
}

void SceneManager::UnloadScene() noexcept
{
    if (!currentScene)
    {
        Logger::Error("No active scene to unload.");
        return;
    }

    currentScene = nullptr;
}

std::unordered_map<std::string, std::unique_ptr<Scene>> SceneManager::scenes;

Scene*   SceneManager::currentScene  = nullptr;
Scene*   SceneManager::nextScene     = nullptr;
Shader*  SceneManager::loadingShader = nullptr;
Mesh*    SceneManager::loadingMesh   = nullptr;
Texture* SceneManager::loadingTex    = nullptr;
float    SceneManager::texAlpha      = 0.0f;