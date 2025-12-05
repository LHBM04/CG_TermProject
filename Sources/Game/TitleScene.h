#pragma once

#include "../Framework/Application.h"
#include "../Framework/Rendering.h"
#include "../Framework/Input.h"
#include "../Framework/Resources.h"
#include "../Framework/Scenes.h"
#include "../Framework/Time.h"
#include "../Framework/Audio.h"
#include <vector>
#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

// 게임 로직 헤더 포함
#include "OBB.h"
#include "PlayerController.h"
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
        cameraSpline               = cameraObject->AddComponent<Spline>();

        mainCamera->GetTransform()->SetPosition(glm::fvec3(0.0f, 30.0f, 20.0f)); // 뷰 조정
        mainCamera->GetTransform()->LookAt(glm::fvec3(0.0f, 0.0f, 0.0f));

        

        // 2. 조명 설정
        Object* const lightObject = AddObject("Directional Light", "Light");
        lightObject->GetTransform()->SetPosition(glm::fvec3(0.0f, 2.0f, 0.0f));
        mainLight = lightObject->AddComponent<Light>();
        mainLight->SetColor(glm::fvec3(1.0f, 1.0f, 1.0f));

        // 3. 미로 구조 생성
        CreateLabyrinth();

        // 4. 플레이어(공) 생성
        CreatePlayer();

        auto bgmPlayer = AddObject("BGM Player", "Audio")->AddComponent<AudioSource>();
        auto bgmClip   = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\Stickerbush Symphony Restored to HD.mp3");
        bgmPlayer->SetLooping(true);
        bgmPlayer->GetTransform()->SetPosition(cameraObject->GetTransform()->GetPosition());
        bgmPlayer->SetClip(bgmClip);
        bgmPlayer->Play();
    }

    virtual void OnUpdate() noexcept override
    {
        mainCamera->GetTransform()->SetPosition(cameraSpline->GetTransform()->GetPosition());
        // (0,0,0) 이 맵 중앙 위치라 맵을 계속 바라보게 하는 용도
        mainCamera->GetTransform()->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));


        // 지금 R키로 공을 위에서 떨어뜨리고 있음.
        // 게임 시작되면 스타트 위치에서 떨어뜨리게 하면 끝
        if (InputManager::IsKeyPressed(Keyboard::R))
        {
            playerObject->GetTransform()->SetPosition(startPosition);
            playerController->setDir(glm::vec3(0.0f, 0.0f, 0.0f));
        }

        // 1번 누르면 메인화면에서 카메라의 이동 
        if (InputManager::IsKeyPressed(Keyboard::D1))
        {
            cameraSpline->deletePoint();
            
            cameraSpline->AddPoint(glm::vec3(0.0f, 5.0f, 9.0f));

            cameraSpline->AddPoint(glm::vec3(-9.0f, 6.0f, 9.0f));
            cameraSpline->AddPoint(glm::vec3(-10.0f, 7.0f, 0.0f));

            cameraSpline->AddPoint(glm::vec3(-11.0f, 8.0f, -10.0f));
            cameraSpline->AddPoint(glm::vec3(0.0f, 9.0f, -10.0f));

            cameraSpline->AddPoint(glm::vec3(10.0f, 10.0f, -10.0f));
            cameraSpline->AddPoint(glm::vec3(10.0f, 11.0f, 0.0f));

            cameraSpline->AddPoint(glm::vec3(9.5f, 12.0f, 6.0f));
            cameraSpline->AddPoint(glm::vec3(9.0f, 13.0f, 12.0f));

            cameraSpline->AddPoint(glm::vec3(7.0f, 14.0f, 13.0f));
            cameraSpline->AddPoint(glm::vec3(5.0f, 15.0f, 15.0f));
        }

        // 메인화면에서 카메라 처음 이동 끝나고 게임시작 버튼이나 게임 제목 등등이 표시된 후에도 계속 조금은 움직이게 설정
        if (cameraSpline->GetTransform()->GetPosition() == glm::vec3(glm::vec3(5.0f, 15.0f, 15.0f)))
        {
            cameraSpline->deletePoint();
            cameraSpline->AddPoint(glm::vec3(5.0f, 15.0f, 15.0f));

            cameraSpline->AddPoint(glm::vec3(3.0f, 15.0f, 15.0f));
            cameraSpline->AddPoint(glm::vec3(1.0f, 15.0f, 15.0f));

            cameraSpline->AddPoint(glm::vec3(-1.0f, 15.0f, 15.0f));
            cameraSpline->AddPoint(glm::vec3(-3.0f, 15.0f, 15.0f));

            cameraSpline->AddPoint(glm::vec3(-5.0f, 15.0f, 15.0f));
            cameraSpline->AddPoint(glm::vec3(-3.0f, 15.0f, 15.0f));

            cameraSpline->AddPoint(glm::vec3(-1.0f, 15.0f, 15.0f));
            cameraSpline->AddPoint(glm::vec3(1.0f, 15.0f, 15.0f));

            cameraSpline->AddPoint(glm::vec3(3.0f, 15.0f, 15.0f));
            cameraSpline->AddPoint(glm::vec3(5.0f, 15.0f, 15.0f));
        }

        // 2번 누르면 게임이 시작되었을 때 카메라의 이동
        if (InputManager::IsKeyPressed(Keyboard::D2))
        {
            cameraSpline->AddPoint(glm::vec3(3.0f, 15.0f, 3.0f));
            cameraSpline->AddPoint(glm::vec3(0.0f, 15.0f, 5.0f));
        }

        if (cameraSpline->GetTransform()->GetPosition() == glm::vec3(glm::vec3(0.0f, 15.0f, 1.0f)) &&
            isPlayerCreated == false)
        {
            isPlayerCreated = true;
            // JSON에서 읽어온 시작 위치 사용
            playerObject->GetTransform()->SetPosition(startPosition);
            playerController->setDir(glm::vec3(0.0f, 0.0f, 0.0f));
        }


        // 마우스 움직임으로 맵 회전
        float dt          = TimeManager::GetDeltaTime();
        float rotateSpeed = 50.0f;
        glm::vec2 mouseDelta = InputManager::GetMousePositionDelta();

        if (mouseDelta.y < 0)
        {
            rotatedAmountX -= rotateSpeed * dt;
        }
        else if (mouseDelta.y > 0)
        {
            rotatedAmountX += rotateSpeed * dt;
        }

        if (mouseDelta.x > 0)
        {
            rotatedAmountZ -= rotateSpeed * dt;
        }
        else if (mouseDelta.x < 0)
        {
            rotatedAmountZ += rotateSpeed * dt;
        }
            
        rotatedAmountX = glm::clamp(rotatedAmountX, -maxRotation, maxRotation);
        rotatedAmountZ = glm::clamp(rotatedAmountZ, -maxRotation, maxRotation);

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


        // 플레이어 물리 업데이트
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
        auto texWood5  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\mapBase.png");
        auto wall  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wall.png");
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

        // ====================================================
        // [변경됨] 레벨 파일 로드 및 생성
        // ====================================================
        std::ifstream file("Assets/Map/level1.json"); // 실행 파일 경로에 level1.json이 있어야 합니다.
        if (!file.is_open())
        {
            std::cout << "[Error] Failed to load level1.json" << std::endl;
            // 파일 로드 실패 시 안전장치 (기존 바닥 하나 생성 등)를 추가할 수도 있습니다.
            return;
        }

        json data;
        file >> data;
        file.close();

        int              mapWidth  = data["width"];
        int              mapHeight = data["height"];
        std::vector<int> tiles     = data["tiles"];

        // 맵의 중심을 (0,0,0)에 맞추기 위한 오프셋 계산
        float offsetX = mapWidth / 2.0f;
        float offsetZ = mapHeight / 2.0f;

        // 1. 타일 생성 (바닥 및 벽)
        // 기존의 통짜 바닥 및 하드코딩된 벽 생성 코드를 모두 대체합니다.
        for (int z = 0; z < mapHeight; ++z)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                int tileType = tiles[z * mapWidth + x];

                // 월드 좌표 계산 (큐브의 중심 위치)
                float posX = (float)x - offsetX + 0.5f; // +0.5f는 그리드 칸 중앙 보정
                float posZ = (float)z - offsetZ + 0.5f;

                // 0(Empty)인 경우 아무것도 만들지 않음 -> 구멍이 됨
                if (tileType == 0)
                    continue;

                // 1(Floor), 2(Wall), 3(Start), 4(Goal)은 모두 바닥이 필요함
                // 바닥 위치: y = -0.5 (두께 1.0인 큐브의 윗면이 0.0이 되도록)
                CreateCube(boardPivot,
                           meshCube,
                           texWood3,
                           glm::vec3(posX, -0.5f, posZ),
                           glm::vec3(1.0f, 1.0f, 1.0f),
                           true); // true = OBB(충돌체) 포함

                // 2(Wall)인 경우 바닥 위에 벽 추가
                if (tileType == 2)
                {
                    // 벽 위치: y = 0.5 (바닥 위)
                    CreateCube(boardPivot,
                               meshCube, wall,
                               glm::vec3(posX, 0.5f, posZ),
                               glm::vec3(1.0f, 1.0f, 1.0f),
                               true);
                }
                // 3(Start)인 경우 시작 위치 저장
                else if (tileType == 3)
                {
                    startPosition = glm::vec3(posX, 2.0f, posZ);
                }
            }
        }
        CreateCube(boardPivot,
                   meshCube,
                   texWood5,
                   glm::vec3(0.0f, -1.5f, 0.0f), // 두꺼워진 만큼 위치를 조금 내려서 윗면 높이를 맞춤
                   glm::vec3(15.0f, 1.0f, 15.0f),
                   true);

        // ====================================================
        // 장식물 (핸들, 프레임 등) - 기존 코드 유지
        // ====================================================

        // 4. 밑바닥 베이스 (Base) - 미로 전체를 받치는 큰 판 (물리 충돌 X)
        CreateCube(nullptr, meshCube, texWood4, glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f), false);

        // X축 핸들
        CreateCube(xHandlePivot, meshCube, texHandle, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 2.0f, 2.0f), false);
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.7f, -3.0f, 0.0f), glm::vec3(18.8f, 0.4f, 0.4f), false);
        CreateCube(nullptr, meshCube, texBar, glm::vec3(8.5f, -1.5f, 0.0f), glm::vec3(0.4f, 3.0f, 0.4f), false);
        CreateCube(nullptr, meshCube, texBar, glm::vec3(-8.5f, -1.5f, 0.0f), glm::vec3(0.4f, 3.0f, 0.4f), false);

        // X축 프레임
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(8.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 17.5f), false);
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(-8.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 17.5f), false);
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(0.0f, 0.0f, 8.5f), glm::vec3(17.5f, 2.0f, 0.4f), false);
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(0.0f, 0.0f, -8.5f), glm::vec3(17.5f, 2.0f, 0.4f), false);

        // Z축 핸들
        CreateCube(zHandlePivot, meshCube, texHandle, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 1.0f), false);
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -3.0f, 0.7f), glm::vec3(0.4f, 0.4f, 19.4f), false);
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -1.5f, 9.5f), glm::vec3(0.4f, 3.0f, 0.4f), false);
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -1.5f, -9.5f), glm::vec3(0.4f, 3.0f, 0.4f), false);

        // Z축 프레임
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

        playerObject->GetTransform()->SetPosition(startPosition);

        // 공의 크기 설정 (기존 반지름 0.4 -> 지름 0.8)
        playerObject->GetTransform()->SetScale(glm::vec3(0.7f));

        MeshRenderer* renderer = playerObject->AddComponent<MeshRenderer>();
        renderer->SetMesh(meshSphere);
        renderer->SetTexture(texBall);

        OBB* obb = playerObject->AddComponent<OBB>();
        // OBB는 반경(Half-extents)을 사용하므로 Scale의 절반인 0.4를 입력
        obb->resize(glm::vec3(0.2f));
        obb->teleport(startPosition);

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
    Spline* cameraSpline = nullptr;

    Light*  mainLight  = nullptr;

    Object*           playerObject     = nullptr;
    PlayerController* playerController = nullptr;
    bool              isPlayerCreated  = false;

    glm::vec3 startPosition;

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