#pragma once

#include "../Framework/Application.h"
#include "../Framework/Rendering.h"
#include "../Framework/Input.h"
#include "../Framework/Resources.h"
#include "../Framework/Scenes.h"
#include "../Framework/Time.h"
#include "../Framework/Audio.h"
#include "../Framework/UI.h"
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
        // 카메라 설정
        Object* const cameraObject = AddGameObject("Main Camera", "Camera");
        cameraObject->GetTransform()->SetPosition(glm::fvec3(0.0f, 30.0f, 20.0f));
        cameraObject->GetTransform()->LookAt(glm::fvec3(0.0f, 0.0f, 0.0f));

        mainCamera = cameraObject->AddComponent<Camera>();
        mainCamera->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));
        
        cameraSpline = cameraObject->AddComponent<Spline>();

        // 조명 설정
        Object* const lightObject = AddGameObject("Directional Light", "Light");
        lightObject->GetTransform()->SetPosition(glm::fvec3(0.0f, 3.0f, 0.0f));
        lightObject->GetTransform()->LookAt(glm::fvec3(0.0f, 0.0f, 0.0f));

        mainLight = lightObject->AddComponent<Light>();
        mainLight->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));
        mainLight->SetColor(glm::fvec3(1.0f, 1.0f, 1.0f));

        // 미로 구조 생성
        CreateLabyrinth();

        auto bgmClip = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\Stickerbush Symphony Restored to HD.mp3");
        bgmPlayer = AddGameObject("BGM Player", "Audio")->AddComponent<AudioSource>();
        bgmPlayer->SetLooping(true);
        bgmPlayer->GetTransform()->SetPosition(cameraObject->GetTransform()->GetPosition());
        bgmPlayer->SetVolume(0.5f);
        bgmPlayer->SetClip(bgmClip);
        bgmPlayer->Play();

        auto hitWallClip = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\hitWall.wav");
        ballSound = AddGameObject("HitWall Sound", "SFX")->AddComponent<AudioSource>();
        ballSound->SetClip(hitWallClip);

        auto resurrectionClip = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\resurrection.wav");
        resurrection          = AddGameObject("Resurrection Sound", "SFX")->AddComponent<AudioSource>();
        resurrection->SetVolume(0.3f);
        resurrection->SetClip(resurrectionClip);

        auto goalClip = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\goal.wav");
        goalSound     = AddGameObject("Goal Sound", "SFX")->AddComponent<AudioSource>();
        goalSound->SetVolume(0.6f);
        goalSound->SetClip(goalClip);

        auto testImage = AddUIObject("Test Image", "UI");
        testImage->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        testImage->GetTransform()->SetScale(glm::vec3(200.0f, 200.0f, 1.0f));

        auto imageComp = testImage->AddComponent<ImageRenderer>();
        imageComp->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\UIObject"));
        imageComp->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Cube.obj"));
        imageComp->SetTexture(ResourceManager::LoadResource<Texture>("Assets\\Textures\\White.png"));
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
            gameStarted = true;
            CreateLabyrinthLevel(1);

            cameraSpline->AddPoint(glm::vec3(3.0f, 15.0f, 3.0f));
            cameraSpline->AddPoint(glm::vec3(0.0f, 20.0f, 5.0f));
        }

        if (cameraSpline->GetTransform()->GetPosition() == glm::vec3(glm::vec3(0.0f, 20.0f, 5.0f)) &&
            isPlayerCreated == false)
        {
            isPlayerCreated = true;
            CreateLabyrinthLevel(1);
            CreatePlayer();
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


        // 플레이어 업데이트
        
        if (playerController && boardPivot)
        {
            // 바닥/벽의 물리 충돌체를 현재 렌더링 위치/회전으로 이동
            UpdatePhysicsWalls();

            // 바닥 Normal 계산
            glm::vec3 groundNormal = boardPivot->GetTransform()->GetUp();
            playerController->SetGroundNormal(groundNormal);

            // 충돌 체크
            for (OBB* wallOBB : wallOBBs)
            {
                playerController->CheckCollision(wallOBB);
            }

            // 소리 재생
            float slidingSoundVolume = glm::clamp(0.0f, glm::length(playerController->GetDir()) / 7, 1.0f);
            float hitVolume = abs(slidingSoundVolume - checkHitWall);

            if (hitVolume > 0.1f)
            {
                ballSound->SetVolume(hitVolume * 2.0f);
                ballSound->SetPitch(hitVolume);
                ballSound->Play();
                checkHitWall = slidingSoundVolume;
            }

            // 낙하 감지
            glm::vec3 playerPos = playerObject->GetTransform()->GetPosition();
            if (playerPos.y < -5.0f)
            {
                SPDLOG_INFO("player fall");
                playerObject->GetTransform()->SetPosition(startPosition);
                resurrection->Play();
            }
            
        }

        // ---------------------------------------------------------------------------------------------------------------------------
        // < 게임 시작 되었을 때 부분 >
        // 
        if (gameStarted)
        {
            static float yScale = 0.0f;

            if (yScale <= 1.0f)
            {
                yScale += 0.002f;

                for (int i{}; i < wallOBBs.size(); ++i)
                {
                    glm::vec3 org = wallOBBs[i]->GetOwner()->GetTransform()->GetScale();
                    wallOBBs[i]->GetOwner()->GetTransform()->SetScale(glm::vec3(org.x, yScale, org.z));
                }
            }


            // 골인했을 때 효과음 재생
            if (playerController)
            {
                glm::vec3   playerPos         = playerObject->GetTransform()->GetPosition();
                static bool isGoalSoundPlayed = false;
                if (abs(goalPosition.x - playerPos.x) < 0.3f && abs(goalPosition.z - playerPos.z) < 0.3f &&
                    !isGoalSoundPlayed)
                {
                    isGoalSoundPlayed = true;
                    goalSound->Play();
                }

                // 지금은 약 5초 뒤에 다시 시작점 되돌아가기로 했음
                // 이걸 다음 맵 바꾸고 그 맵의 시작점으로 가게 하면 될듯
                if (isGoalSoundPlayed)
                {
                    static float time = 0.0f;
                    time += 0.01f;

                    if (time > 5.0f)
                    {
                        playerObject->GetTransform()->SetPosition(startPosition);
                        isGoalSoundPlayed = false;
                        time = 0.0f;
                    }
                }
            }
            
        }
    }

private:

    void CreateLabyrinth()
    {
        // 피봇 생성 (회전 중심점)
        boardPivot   = AddGameObject("BoardPivot", "Pivot");
        xFramePivot  = AddGameObject("XFramePivot", "Pivot");
        zFramePivot  = AddGameObject("ZFramePivot", "Pivot");
        xHandlePivot = AddGameObject("XHandlePivot", "Pivot");
        xHandlePivot->GetTransform()->SetPosition(glm::vec3(10.5f, -3.0f, 0.0f));
        zHandlePivot = AddGameObject("ZHandlePivot", "Pivot");
        zHandlePivot->GetTransform()->SetPosition(glm::vec3(0.0f, -3.0f, 10.5f));

        CreateCube(boardPivot,
                   meshCube,
                   texWood5,
                   glm::vec3(0.0f, -1.5f, 0.0f),
                   glm::vec3(15.0f, 1.0f, 15.0f),
                   false);
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

        for (int i{}; i < wallOBBs.size(); ++i)
        {
            glm::vec3 org = wallOBBs[i]->GetOwner()->GetTransform()->GetScale();
            wallOBBs[i]->GetOwner()->GetTransform()->SetScale(glm::vec3(org.x, 0.0f, org.z));
        }
    }

    void CreateLabyrinthLevel(int levelNum)
    {
        if (levelNum == 1)
        {
            std::ifstream file("Assets/Map/level1.json");
            if (!file.is_open())
            {
                std::cout << "[Error] Failed to load level1.json" << std::endl;
                return;
            }

            json data;
            file >> data;
            file.close();

            int              mapWidth  = data["width"];
            int              mapHeight = data["height"];
            std::vector<int> tiles     = data["tiles"];

            float offsetX = mapWidth / 2.0f;
            float offsetZ = mapHeight / 2.0f;

            // 타일 생성 (바닥 및 벽)
            for (int z = 0; z < mapHeight; ++z)
            {
                for (int x = 0; x < mapWidth; ++x)
                {
                    int tileType = tiles[z * mapWidth + x];

                    float posX = (float)x - offsetX + 0.5f;
                    float posZ = (float)z - offsetZ + 0.5f;

                    // 0 Empty
                    if (tileType == 0)
                        continue;

                    CreateCube(boardPivot,
                               meshCube,
                               texWood3,
                               glm::vec3(posX, -0.5f, posZ),
                               glm::vec3(1.0f, 1.0f, 1.0f),
                               true); // true = OBB(충돌체) 포함

                    // 2 Wall
                    if (tileType == 2)
                    {
                        CreateCube(boardPivot,
                                   meshCube,
                                   wall,
                                   glm::vec3(posX, 0.5f, posZ),
                                   glm::vec3(1.0f, 1.0f, 1.0f),
                                   true);
                    }

                    // 3 Start
                    else if (tileType == 3)
                    {
                        startPosition = glm::vec3(posX, 2.0f, posZ);
                        CreateCube(boardPivot,
                                   meshCube,
                                   texRed,
                                   glm::vec3(posX, -0.5f, posZ),
                                   glm::vec3(1.0f, 1.2f, 1.0f),
                                   false);
                    }

                    // 4 Goal
                    else if (tileType == 4)
                    {
                        goalPosition = glm::vec3(posX, 0.0f, posZ);
                        CreateCube(boardPivot,
                                   meshCube,
                                   texGreen,
                                   glm::vec3(posX, -0.5f, posZ),
                                   glm::vec3(1.0f, 1.2f, 1.0f),
                                   false);
                    }
                }
            }
        }
        else if (levelNum == 2)
        {
            std::ifstream file("Assets/Map/level2.json");
            if (!file.is_open())
            {
                std::cout << "[Error] Failed to load level2.json" << std::endl;
                return;
            }

            json data;
            file >> data;
            file.close();

            int              mapWidth  = data["width"];
            int              mapHeight = data["height"];
            std::vector<int> tiles     = data["tiles"];

            float offsetX = mapWidth / 2.0f;
            float offsetZ = mapHeight / 2.0f;

            // 타일 생성 (바닥 및 벽)
            for (int z = 0; z < mapHeight; ++z)
            {
                for (int x = 0; x < mapWidth; ++x)
                {
                    int tileType = tiles[z * mapWidth + x];

                    float posX = (float)x - offsetX + 0.5f;
                    float posZ = (float)z - offsetZ + 0.5f;

                    // 0 Empty
                    if (tileType == 0)
                        continue;

                    CreateCube(boardPivot,
                               meshCube,
                               texWood3,
                               glm::vec3(posX, -0.5f, posZ),
                               glm::vec3(1.0f, 1.0f, 1.0f),
                               true); // true = OBB(충돌체) 포함

                    // 2 Wall
                    if (tileType == 2)
                    {
                        CreateCube(boardPivot,
                                   meshCube,
                                   wall,
                                   glm::vec3(posX, 0.5f, posZ),
                                   glm::vec3(1.0f, 1.0f, 1.0f),
                                   true);
                    }

                    // 3 Start
                    else if (tileType == 3)
                    {
                        startPosition = glm::vec3(posX, 2.0f, posZ);
                        CreateCube(boardPivot,
                                   meshCube,
                                   texRed,
                                   glm::vec3(posX, -0.5f, posZ),
                                   glm::vec3(1.0f, 1.2f, 1.0f),
                                   false);
                    }

                    // 4 Goal
                    else if (tileType == 4)
                    {
                        goalPosition = glm::vec3(posX, 0.0f, posZ);
                        CreateCube(boardPivot,
                                   meshCube,
                                   texGreen,
                                   glm::vec3(posX, -0.5f, posZ),
                                   glm::vec3(1.0f, 1.2f, 1.0f),
                                   false);
                    }
                }
            }
        }
    }

    void CreatePlayer()
    {
        auto meshSphere =
                ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Ball.obj"); // 구체 모델이 없다면 큐브로 대체
        auto texBall = ResourceManager::LoadResource<Texture>(
                "Assets\\Textures\\Poketball.png"); // 혹은 Rendering/Poketball.png

        playerObject = AddGameObject("Player", "Player");

        playerObject->GetTransform()->SetPosition(startPosition);

        // 공의 크기 설정 (기존 반지름 0.4 -> 지름 0.8)
        playerObject->GetTransform()->SetScale(glm::vec3(0.7f));

        MeshRenderer* renderer = playerObject->AddComponent<MeshRenderer>();
        renderer->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));
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
        Object* obj = AddGameObject("CubePart", isWall ? "Wall" : "Deco");
        if (parent)
            obj->GetTransform()->SetParent(parent->GetTransform());

        obj->GetTransform()->SetPosition(position);
        obj->GetTransform()->SetScale(scale);

        MeshRenderer* renderer = obj->AddComponent<MeshRenderer>();
        renderer->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));
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

    // 사운드 관련
    AudioSource*      ballSound        = nullptr;
    AudioSource*      bgmPlayer        = nullptr;
    AudioSource*      resurrection     = nullptr;
    AudioSource*      goalSound        = nullptr;
    float             checkHitWall     = 0.0f;

    glm::vec3 startPosition;
    glm::vec3 goalPosition;

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


    // 텍스쳐를 실행 도중에 로드하면 렉 걸려서 처음 실행될 때 미리 다 로드하게 함
    Mesh* meshCube = ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Cube.obj");

    Texture* wall     = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wall.png");
    Texture* texWood1  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture1.png");
    Texture* texWood2  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture2.png");
    Texture* texWood3  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture3.png");
    Texture* texWood4  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture4.png");
    Texture* texWood5  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\mapBase.png");
    Texture* texHandle = ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle.png");
    Texture* texBar    = ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle_bar.png");

    Texture* texGreen = ResourceManager::LoadResource<Texture>("Assets\\Textures\\Green.png");
    Texture* texRed = ResourceManager::LoadResource<Texture>("Assets\\Textures\\Red.png");

    // 게임 플레이 진입 여부 확인용
    bool gameStarted = false;
};