#pragma once

#include "../Framework/Application.h"
#include "../Framework/Rendering.h"
#include "../Framework/Input.h"
#include "../Framework/Resources.h"
#include "../Framework/Scenes.h"
#include "../Framework/Time.h"
#include <vector>
#include "Spline.h"

/**
 * @class TitleScene
 *
 * @brief Labyrinth 게임 씬
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
        // 1. 카메라 설정
        Object* const cameraObject = AddObject("Main Camera", "Camera");
        mainCamera                 = cameraObject->AddComponent<Camera>();
        // 미로 전체가 보이도록 카메라 위치 조정 (위에서 아래로)
        mainCamera->GetTransform()->SetPosition(glm::fvec3(0.0f, 20.0f, 20.0f));
        mainCamera->GetTransform()->LookAt(glm::fvec3(0.0f, 0.0f, 0.0f));

        // 2. 조명 설정
        Object* const lightObject = AddObject("Directional Light", "Light");
        lightObject->GetTransform()->SetPosition(glm::fvec3(0.0f, 10.0f, 0.0f));
        mainLight = lightObject->AddComponent<Light>();
        mainLight->SetColor(glm::fvec3(1.0f, 1.0f, 1.0f));

        // 3. 미로 구조 생성
        CreateLabyrinth();
    }

    virtual void OnUpdate() noexcept override
    {
        float dt          = TimeManager::GetDeltaTime();
        float rotateSpeed = 30.0f;

        if (InputManager::IsKeyHeld(Keyboard::Up))
        {
            rotatedAmountX += rotateSpeed * dt;
        }
        if (InputManager::IsKeyHeld(Keyboard::Down))
        {
            rotatedAmountX -= rotateSpeed * dt;
        }
        if (InputManager::IsKeyHeld(Keyboard::Right))
        {
            rotatedAmountZ -= rotateSpeed * dt;
        }
        if (InputManager::IsKeyHeld(Keyboard::Left))
        {
            rotatedAmountZ += rotateSpeed * dt;
        }

        // 회전 각도 제한 (-10 ~ 10도)
        rotatedAmountX = glm::clamp(rotatedAmountX, -maxRotation, maxRotation);
        rotatedAmountZ = glm::clamp(rotatedAmountZ, -maxRotation, maxRotation);

        // 회전 적용
        // 1) 보드(맵)는 X축과 Z축 회전이 모두 적용됨 (Pitch * Roll)
        if (boardPivot)
        {
            // 오일러 각을 사용하여 회전 설정 (순서 주의: 여기서는 X 먼저 적용 후 Z)
            // 쿼터니언을 직접 만들어도 되지만, 프레임워크의 SetRotation은 Euler(Degree)를 받음
            // 프레임워크 Transform::SetRotation 구현에 따라 다르지만,
            // 여기서는 독립적인 축 회전을 위해 쿼터니언을 직접 계산하지 않고 간단히 오일러로 접근하거나
            // 원본 로직(Pitch * Roll)을 따르기 위해 아래와 같이 설정 가능

            // Transform::SetRotation은 vec3(x, y, z) 오일러 각을 받습니다.
            boardPivot->GetTransform()->SetRotation(glm::vec3(rotatedAmountX, 0.0f, rotatedAmountZ));
        }

        // X축 프레임회전
        if (xFramePivot)
        {
            xFramePivot->GetTransform()->SetRotation(glm::vec3(rotatedAmountX, 0.0f, 0.0f));
        }

        // Z축 프레임 회전
        if (zFramePivot)
        {
            zFramePivot->GetTransform()->SetRotation(glm::vec3(0.0f, 0.0f, rotatedAmountZ));
        }
    }

private:
    void CreateLabyrinth()
    {
        auto meshCube = ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Cube.obj");

        auto texWood1  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture1.png");
        auto texWood2  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture2.png");
        auto texWood3  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture3.png");
        auto texWood4  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture4.png");
        auto texHandle = ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle.png");
        auto texBar    = ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle_bar.png");

        // 피봇들
        boardPivot  = AddObject("BoardPivot", "Pivot");
        xFramePivot = AddObject("XFramePivot", "Pivot");
        zFramePivot = AddObject("ZFramePivot", "Pivot");


        // 보드판
        for (int i = -7; i <= 7; ++i)
        {
            for (int j = -7; j <= 7; ++j)
            {
                // 외곽 벽 생성
                if (i == -7 || i == 7 || j == -7 || j == 7)
                {
                    CreateCube(boardPivot,
                               meshCube,
                               texWood3,
                               glm::vec3((float)i, 0.5f, (float)j),
                               glm::vec3(1.0f, 1.0f, 1.0f)); // 0.5 * 2
                }
            }
        }

        // 내부 장애물 벽
        int wallIdx[10][2] = {
                {-5, -6}, {-4, -6}, {-3, -6}, {-2, -6}, {-1, -6}, {-1, -6}, {2, -4}, {1, -4}, {0, -4}, {-1, -4}};
        for (int i = 0; i < 10; ++i)
        {
            CreateCube(boardPivot,
                       meshCube,
                       texHandle,
                       glm::vec3(wallIdx[i][0], 0.5f, wallIdx[i][1]),
                       glm::vec3(1.0f, 1.0f, 1.0f));
        }

        // 바닥판
        CreateCube(boardPivot,
                   meshCube,
                   texWood3,
                   glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(15.0f, 0.1f, 15.0f)); // 7.5 * 2. 높이는 얇게 설정

        CreateCube(
                nullptr, meshCube, texWood4, glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f)); // 10.0 * 2

        // X축 손잡이랑 바닥 파이프들
        CreateCube(nullptr, meshCube, texHandle, glm::vec3(10.5f, -3.0f, 0.0f), glm::vec3(1.0f, 2.0f, 2.0f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.7f, -3.0f, 0.0f), glm::vec3(18.8f, 0.4f, 0.4f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(8.5f, -1.5f, 0.0f), glm::vec3(0.4f, 3.0f, 0.4f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(-8.5f, -1.5f, 0.0f), glm::vec3(0.4f, 3.0f, 0.4f));
        
        // X축 프레임
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(8.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 17.5f));
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(-8.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 17.5f));
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(0.0f, 0.0f, 8.5f), glm::vec3(17.5f, 2.0f, 0.4f));
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(0.0f, 0.0f, -8.5f), glm::vec3(17.5f, 2.0f, 0.4f));

        // Z축 손잡이랑 바닥파이프들
        CreateCube(nullptr, meshCube, texHandle, glm::vec3(0.0f, -3.0f, 10.5f), glm::vec3(2.0f, 2.0f, 1.0f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -3.0f, 0.7f), glm::vec3(0.4f, 0.4f, 19.4f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -1.5f, 9.5f), glm::vec3(0.4f, 3.0f, 0.4f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -1.5f, -9.5f), glm::vec3(0.4f, 3.0f, 0.4f));

        // Z축 프레임
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(9.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 19.5f));
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(-9.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 19.5f));
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(0.0f, 0.0f, 9.5f), glm::vec3(19.5f, 2.0f, 0.4f));
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(0.0f, 0.0f, -9.5f), glm::vec3(19.5f, 2.0f, 0.4f));
    }

    // 큐브 생성 헬퍼 함수
    void CreateCube(Object* parent, Mesh* mesh, Texture* texture, glm::vec3 position, glm::vec3 scale)
    {
        Object* obj = AddObject("CubePart", "Labyrinth");
        if (parent)
        {
            obj->GetTransform()->SetParent(parent->GetTransform());
        }

        obj->GetTransform()->SetPosition(position);
        obj->GetTransform()->SetScale(scale);

        MeshRenderer* renderer = obj->AddComponent<MeshRenderer>();
        renderer->SetMesh(mesh);
        renderer->SetTexture(texture);
    }

private:
    Camera* mainCamera = nullptr;
    Light*  mainLight  = nullptr;

    // 회전 중심점들
    Object* boardPivot  = nullptr; // 미로 바닥 + 벽
    Object* xFramePivot = nullptr; // X축 프레임
    Object* zFramePivot = nullptr; // Z축 프레임

    // 회전 상태
    float       rotatedAmountX = 0.0f;
    float       rotatedAmountZ = 0.0f;
    const float maxRotation    = 10.0f;
};