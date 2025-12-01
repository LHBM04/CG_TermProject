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
        // Scene에 입장하면 반드시 testCamera가 반드시 있어야 함.
        testCamera = AddObject("Test Camera", "Main Camera")->AddComponent<Camera>();
        testCamera->GetTransform()->SetPosition(glm::fvec3(0.0f, 0.0f, 5.0f));
        testCamera->GetTransform()->SetRotation(glm::fvec3(0.0f, -90.0f, 0.0f));

        testCamera_spline = testCamera->GetOwner()->AddComponent<Spline>();
        testCamera_spline->AddPoint(glm::vec3(40.0f, 35.0f, 0.0f));
        testCamera_spline->AddPoint(glm::vec3(0.0f, 30.0f, 40.0f));
        testCamera_spline->AddPoint(glm::vec3(-40.0f, 25.0f, 0.0f));
        testCamera_spline->AddPoint(glm::vec3(0.0f, 20.0f, -40.0f));
        testCamera_spline->AddPoint(glm::vec3(40.0f, 15.0f, 0.0f));
        testCamera_spline->AddPoint(glm::vec3(0.0f, 20.0f, 40.0f));
        testCamera_spline->AddPoint(glm::vec3(20.0f, 20.0f, 20.0f));

        

        testRenderer = AddObject("Test Object", "Object")->AddComponent<MeshRenderer>();
        testRenderer->GetTransform()->SetPosition(glm::fvec3(0.0f, 0.0f, 0.0f)); 
        testRenderer->GetTransform()->SetRotation(glm::fvec3(0.0f, 45.0f, 0.0f));

        testRenderer->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Cube.obj"));
    }

    virtual void OnUpdate() noexcept override
    {
        testCamera->GetTransform()->SetPosition(testCamera_spline->GetTransform()->GetPosition());

        const glm::fvec3 tmp = testCamera->GetTransform()->GetPosition();
        SPDLOG_INFO("cameraPos: {}, {}, {}", tmp.x, tmp.y, tmp.z);

        if (InputManager::IsKeyPressed(Keyboard::Enter))
        {
            Application::Quit();
        }

        const float      scalar = TimeManager::GetDeltaTime() * 50.0f;
        const glm::fvec3 curPos = testRenderer->GetTransform()->GetPosition();

        if (InputManager::IsKeyHeld(Keyboard::W))
        {
            testRenderer->GetTransform()->SetPosition(curPos + scalar * glm::fvec3(0.0f, 0.0f, -0.1f));
        }
        if (InputManager::IsKeyHeld(Keyboard::S))
        {
            testRenderer->GetTransform()->SetPosition(curPos + scalar * glm::fvec3(0.0f, 0.0f, 0.1f));
        }
        if (InputManager::IsKeyHeld(Keyboard::A))
        {
            testRenderer->GetTransform()->SetPosition(curPos + scalar * glm::fvec3(-0.1f, 0.0f, 0.0f));
        }
        if (InputManager::IsKeyHeld(Keyboard::D))
        {
            testRenderer->GetTransform()->SetPosition(curPos + scalar * glm::fvec3(0.1f, 0.0f, 0.0f));
        }
    }

private:
    /**
     * @brief 테스트에 사용할 카메라.
     */
    Camera* testCamera = nullptr;

    /**
     * @brief 카메라 이동 곡선
     */
    Spline* testCamera_spline = nullptr;

    /**
     * @brief 테스트에 사용할 메쉬 렌더러.
     */
    MeshRenderer* testRenderer = nullptr;

    /**
     * @brief 
     */
    Object* testObject = nullptr;
};