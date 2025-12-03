#pragma once

#include "../Framework/Application.h"
#include "../Framework/Rendering.h"
#include "../Framework/Input.h"
#include "../Framework/Resources.h"
#include "../Framework/Scenes.h"
#include "../Framework/Time.h"
#include <vector>
#include "Spline.h"

// 게임 로직 헤더 포함
#include "OBB.h"
#include "PlayerController.h"

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
        mainCamera->GetTransform()->SetPosition(glm::fvec3(0.0f, 30.0f, 20.0f)); // 뷰 조정
        mainCamera->GetTransform()->LookAt(glm::fvec3(0.0f, 0.0f, 0.0f));

        // 2. 조명 설정
        Object* const lightObject = AddObject("Directional Light", "Light");
        lightObject->GetTransform()->SetPosition(glm::fvec3(0.0f, 10.0f, 0.0f));
        mainLight = lightObject->AddComponent<Light>();
        mainLight->SetColor(glm::fvec3(1.0f, 1.0f, 1.0f));

        // 3. 미로 구조 생성
        CreateLabyrinth();

        // 4. 플레이어(공) 생성
        CreatePlayer();
    }

    virtual void OnUpdate() noexcept override
    {
        if (InputManager::IsKeyPressed(Keyboard::R))
        {
            playerObject->GetTransform()->SetPosition(glm::vec3(0.0f, 20.0f, 0.0f));
        }

        float dt          = TimeManager::GetDeltaTime();
        float rotateSpeed = 30.0f;

        // 키 입력 처리
        if (InputManager::IsKeyHeld(Keyboard::Up))
            rotatedAmountX -= rotateSpeed * dt;
        if (InputManager::IsKeyHeld(Keyboard::Down))
            rotatedAmountX += rotateSpeed * dt;
        if (InputManager::IsKeyHeld(Keyboard::Right))
            rotatedAmountZ -= rotateSpeed * dt;
        if (InputManager::IsKeyHeld(Keyboard::Left))
            rotatedAmountZ += rotateSpeed * dt;

        rotatedAmountX = glm::clamp(rotatedAmountX, -maxRotation, maxRotation);
        rotatedAmountZ = glm::clamp(rotatedAmountZ, -maxRotation, maxRotation);

        // 1) 보드(맵) 회전 적용
        if (boardPivot)
        {
            boardPivot->GetTransform()->SetRotation(glm::vec3(rotatedAmountX, 0.0f, rotatedAmountZ));
        }

        // 장식물 회전
        if (xFramePivot)
            xFramePivot->GetTransform()->SetRotation(glm::vec3(rotatedAmountX, 0.0f, 0.0f));
        if (xHandlePivot)
            xHandlePivot->GetTransform()->SetRotation(glm::vec3(rotatedAmountX, 0.0f, 0.0f));
        if (zFramePivot)
            zFramePivot->GetTransform()->SetRotation(glm::vec3(0.0f, 0.0f, rotatedAmountZ));
        if (zHandlePivot)
            zHandlePivot->GetTransform()->SetRotation(glm::vec3(0.0f, 0.0f, rotatedAmountZ));

        // 2) 플레이어 물리 업데이트
        if (playerController && boardPivot)
        {
            // 1. 바닥/벽의 물리 충돌체를 현재 렌더링 위치/회전으로 이동
            UpdatePhysicsWalls();

            // 2. 바닥 Normal 계산
            glm::vec3 groundNormal = boardPivot->GetTransform()->GetUp();
            playerController->SetGroundNormal(groundNormal);

            // 3. 충돌 체크
            for (OBB* wallOBB : wallOBBs)
            {
                playerController->CheckCollision(wallOBB);
            }
        }
    }

private:
    void CreateLabyrinth()
    {
        auto meshCube = ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Cube.obj");

        // 텍스처 로드
        auto texWood1  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture1.png");
        auto texWood2  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture2.png");
        auto texWood3  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture3.png");
        auto texWood4  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture4.png");
        auto texHandle = ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle.png");
        auto texBar    = ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle_bar.png");

        // 피봇 생성 (회전 중심점)
        boardPivot   = AddObject("BoardPivot", "Pivot");
        xFramePivot  = AddObject("XFramePivot", "Pivot");
        zFramePivot  = AddObject("ZFramePivot", "Pivot");
        xHandlePivot = AddObject("XHandlePivot", "Pivot");
        xHandlePivot->GetTransform()->SetPosition(glm::vec3(10.5f, -3.0f, 0.0f));
        zHandlePivot = AddObject("ZHandlePivot", "Pivot");
        zHandlePivot->GetTransform()->SetPosition(glm::vec3(0.0f, -3.0f, 10.5f));

        // 1. 보드판 바닥 (Floor)
        // 기존 radius(7.5) -> Scale(15.0)
        // 높이는 얇게 0.2로 설정
        CreateCube(boardPivot,
                   meshCube,
                   texWood3,
                   glm::vec3(0.0f, -0.5f, 0.0f), // 두꺼워진 만큼 위치를 조금 내려서 윗면 높이를 맞춤
                   glm::vec3(15.0f, 1.0f, 15.0f),
                   true);

        // 2. 보드판 외곽 벽 및 내부 벽
        // 기존 radius(0.5) -> Scale(1.0) : 이렇게 해야 1칸을 꽉 채웁니다.
        glm::vec3 wallScale = glm::vec3(1.0f, 1.0f, 1.0f);

        for (int i = -7; i <= 7; ++i)
        {
            for (int j = -7; j <= 7; ++j)
            {
                // 외곽 벽
                if (i == -7 || i == 7 || j == -7 || j == 7)
                {
                    // 위치는 좌표 그대로 사용 (i, 0.5, j)
                    CreateCube(boardPivot, meshCube, texWood3, glm::vec3((float)i, 0.5f, (float)j), wallScale, true);
                }
            }
        }

        // 3. 내부 장애물 벽
        // 마찬가지로 Scale을 1.0으로 설정
        int wallIdx[10][2] = {
                {-5, -6}, {-4, -6}, {-3, -6}, {-2, -6}, {-1, -6}, {-1, -6}, {2, -4}, {1, -4}, {0, -4}, {-1, -4}};

        for (int i = 0; i < 10; ++i)
        {
            CreateCube(boardPivot, meshCube, texHandle, glm::vec3(wallIdx[i][0], 0.5f, wallIdx[i][1]), wallScale, true);
        }

        // 4. 밑바닥 베이스 (Base)
        // 기존 radius(10.0, 0.5, 10.0) -> Scale(20.0, 1.0, 20.0)
        CreateCube(nullptr, meshCube, texWood4, glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f), false);

        // ====================================================
        // 장식물 (핸들, 프레임 등) - 모든 Scale 값을 기존 radius * 2로 설정
        // ====================================================

        // X축 핸들 (기존 radius: 0.5, 1.0, 1.0 -> Scale: 1.0, 2.0, 2.0)
        CreateCube(xHandlePivot, meshCube, texHandle, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 2.0f, 2.0f), false);

        // 바 (radius: 9.4, 0.2, 0.2 -> Scale: 18.8, 0.4, 0.4)
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.7f, -3.0f, 0.0f), glm::vec3(18.8f, 0.4f, 0.4f), false);

        // 연결부 (radius: 0.2, 1.5, 0.2 -> Scale: 0.4, 3.0, 0.4)
        CreateCube(nullptr, meshCube, texBar, glm::vec3(8.5f, -1.5f, 0.0f), glm::vec3(0.4f, 3.0f, 0.4f), false);
        CreateCube(nullptr, meshCube, texBar, glm::vec3(-8.5f, -1.5f, 0.0f), glm::vec3(0.4f, 3.0f, 0.4f), false);

        // X축 프레임 (radius: 0.2, 1.0, 8.75 -> Scale: 0.4, 2.0, 17.5)
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(8.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 17.5f), false);
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(-8.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 17.5f), false);

        // X축 프레임 가로 (radius: 8.75, 1.0, 0.2 -> Scale: 17.5, 2.0, 0.4)
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(0.0f, 0.0f, 8.5f), glm::vec3(17.5f, 2.0f, 0.4f), false);
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(0.0f, 0.0f, -8.5f), glm::vec3(17.5f, 2.0f, 0.4f), false);

        // Z축 핸들 (radius: 1.0, 1.0, 0.5 -> Scale: 2.0, 2.0, 1.0)
        CreateCube(zHandlePivot, meshCube, texHandle, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 1.0f), false);

        // Z축 바 (radius: 0.2, 0.2, 9.7 -> Scale: 0.4, 0.4, 19.4)
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -3.0f, 0.7f), glm::vec3(0.4f, 0.4f, 19.4f), false);

        // Z축 연결부
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -1.5f, 9.5f), glm::vec3(0.4f, 3.0f, 0.4f), false);
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -1.5f, -9.5f), glm::vec3(0.4f, 3.0f, 0.4f), false);

        // Z축 프레임 (radius: 0.2, 1.0, 9.75 -> Scale: 0.4, 2.0, 19.5)
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(9.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 19.5f), false);
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(-9.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 19.5f), false);
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(0.0f, 0.0f, 9.5f), glm::vec3(19.5f, 2.0f, 0.4f), false);
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(0.0f, 0.0f, -9.5f), glm::vec3(19.5f, 2.0f, 0.4f), false);
    }

    void CreatePlayer()
    {
        auto meshSphere =
                ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Ball.obj"); // 구체 모델이 없다면 큐브로 대체
        auto texBall = ResourceManager::LoadResource<Texture>(
                "Assets\\Textures\\Poketball.png"); // 혹은 Rendering/Poketball.png

        playerObject = AddObject("Player", "Player");

        // 높이를 조금 더 높여서(5.0f) 바닥에 닿지 않고 떨어지도록 시작
        playerObject->GetTransform()->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));

        // 공의 크기 설정 (기존 반지름 0.4 -> 지름 0.8)
        playerObject->GetTransform()->SetScale(glm::vec3(0.7f));

        MeshRenderer* renderer = playerObject->AddComponent<MeshRenderer>();
        renderer->SetMesh(meshSphere);
        renderer->SetTexture(texBall);

        OBB* obb = playerObject->AddComponent<OBB>();
        // OBB는 반경(Half-extents)을 사용하므로 Scale의 절반인 0.4를 입력
        obb->resize(glm::vec3(0.2f));
        obb->teleport(glm::vec3(0.0f, 5.0f, 0.0f));

        // 컨트롤러 추가
        playerController = playerObject->AddComponent<PlayerController>();
        // 물리 값 설정
        playerController->SetGravityScale(1.0f); // 중력 좀 더 세게
        playerController->SetSlopeBoost(2.5f);
        playerController->SetSpeedScale(1.3f);
    }

    // 큐브 생성 헬퍼 함수
    void CreateCube(Object* parent, Mesh* mesh, Texture* texture, glm::vec3 position, glm::vec3 scale, bool isWall)
    {
        Object* obj = AddObject("CubePart", isWall ? "Wall" : "Deco");
        if (parent)
            obj->GetTransform()->SetParent(parent->GetTransform());

        obj->GetTransform()->SetPosition(position);
        obj->GetTransform()->SetScale(scale);

        MeshRenderer* renderer = obj->AddComponent<MeshRenderer>();
        renderer->SetMesh(mesh);
        renderer->SetTexture(texture);

        // [중요] 벽이나 바닥일 경우 OBB(충돌체) 추가
        if (isWall)
        {
            OBB* obb = obj->AddComponent<OBB>();
            obb->resize(scale * 0.5f); // Scale이 곧 반지름(Half-extent) 역할
            obb->teleport(position);
            wallOBBs.push_back(obb);
        }
    }

    void UpdatePhysicsWalls()
    {
        for (OBB* obb : wallOBBs)
        {
            Transform* tr = obb->GetTransform();
            if (tr)
            {
                // 1. 월드 행렬 가져오기
                glm::mat4 worldMat = tr->GetWorldMatrix();

                // 2. 위치 추출
                glm::vec3 pos = glm::vec3(worldMat[3]);

                // 회전 추출 (스케일 제거 후 순수 회전만 뽑기)
                glm::vec3 axisX = glm::normalize(glm::vec3(worldMat[0]));
                glm::vec3 axisY = glm::normalize(glm::vec3(worldMat[1]));
                glm::vec3 axisZ = glm::normalize(glm::vec3(worldMat[2]));

                glm::mat3 rotMat;
                rotMat[0]     = axisX;
                rotMat[1]     = axisY;
                rotMat[2]     = axisZ;
                glm::quat rot = glm::quat_cast(rotMat);

                // 동기화
                obb->teleport(pos);
                obb->setRotation(rot);
            }
        }
    }


private:
    Camera* mainCamera = nullptr;
    Light*  mainLight  = nullptr;

    Object*           playerObject     = nullptr;
    PlayerController* playerController = nullptr;

    // 회전 중심점들
    Object* boardPivot  = nullptr; // 미로 바닥 + 벽
    Object* xFramePivot = nullptr; // X축 프레임
    Object* zFramePivot = nullptr; // Z축 프레임
    Object* xHandlePivot = nullptr; // X축 손잡이의 피봇
    Object* zHandlePivot = nullptr; // Z축 손잡이의 피봇

    std::vector<OBB*> wallOBBs; // 충돌 체크 최적화를 위한 벽 리스트

    // 회전 상태
    float       rotatedAmountX = 0.0f;
    float       rotatedAmountZ = 0.0f;
    const float maxRotation    = 10.0f;
};