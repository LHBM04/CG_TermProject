#pragma once

#include "../Framework/Application.h"
#include "../Framework/Rendering.h"
#include "../Framework/Input.h"
#include "../Framework/Resources.h"
#include "../Framework/Scenes.h"
#include "../Framework/Time.h"
#include "Spline.h"

/**
 * @class TitleScene
 *
 * @brief 타이틀 씬을 정의합니다.
 */
class TitleScene final : public Scene
{
public:
    explicit TitleScene() noexcept
        : Scene()
    {

    }

    virtual ~TitleScene() noexcept override
    {

    }

    virtual void OnEnter() noexcept override
    {
        Object* const cameraObject = AddObject("Main Camera", "Camera");

        mainCamera = cameraObject->AddComponent<Camera>();
        mainCamera->GetTransform()->SetPosition(glm::fvec3(0.0f, 5.5f, 7.0f));
        mainCamera->GetTransform()->LookAt(glm::fvec3(0.0f, 0.0f, 0.0f));

        Object* const lightObject = AddObject("Directional Light", "Light");
        lightObject->GetTransform()->SetPosition(glm::fvec3(0.0f, 2.5f, 3.0f));
        
        MeshRenderer* const lightRenderer = lightObject->AddComponent<MeshRenderer>();
        lightRenderer->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Cube.obj"));
        lightRenderer->SetTexture(ResourceManager::LoadResource<Texture>("Assets\\Textures\\White.png"));

        mainLight = lightObject->AddComponent<Light>();
        mainLight->SetColor(glm::fvec3(1.0f, 1.0f, 1.0f));

        Object* const testObject = AddObject("Test Object", "Object");
        testObject->GetTransform()->SetPosition(glm::fvec3(0.0f, 0.0f, 0.0f));

        MeshRenderer* objRenderer = testObject->AddComponent<MeshRenderer>();
        objRenderer->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Cube.obj"));
        objRenderer->SetTexture(ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle_bar.png"));
    }
    

    virtual void OnUpdate() noexcept override
    {
        static float angle    = 0.0f;
        static float distance = 3.0f;

        const float     rotationSpeed = 45.0f;
        const glm::vec3 center(0.0f, 0.0f, 0.0f);

        if (InputManager::IsKeyHeld(Keyboard::N))
        {
            distance += 10.0f * TimeManager::GetDeltaTime();
        }
        if (InputManager::IsKeyHeld(Keyboard::F))
        {
            distance -= 10.0f * TimeManager::GetDeltaTime();
            if (distance < 0.5f)
            {
                distance = 0.5f;
            }
        }

        angle += rotationSpeed * TimeManager::GetDeltaTime();
        float radianAngle = glm::radians(angle);
        float currentY    = center.y + (distance * 0.5f);

        float h_radius = distance * 0.866f;

        float camX = center.x + sin(radianAngle) * h_radius;
        float camZ = center.z + cos(radianAngle) * h_radius;

        mainLight->GetTransform()->SetPosition(glm::vec3(camX, currentY, camZ));
        mainLight->GetTransform()->LookAt(center);
    }

private:
    /**
     * @brief 해당 씬의 메인 카메라.
     */
    Camera* mainCamera = nullptr;

    /**
     * @brief 해당 씬의 메인 라이트.
     */
    Light* mainLight = nullptr;
};