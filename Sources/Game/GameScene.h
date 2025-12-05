#pragma once

#include "../Framework/Audio.h"
#include "../Framework/Input.h"
#include "../Framework/Resources.h"
#include "../Framework/Scenes.h"
#include "../Framework/Time.h"
#include "GameManager.h"

#include "OBB.h"
#include "PlayerController.h"
#include "nlohmann/json.hpp"

class GameScene : public Scene
{
public:
    explicit GameScene() noexcept;
    virtual ~GameScene() noexcept override;

    void OnEnter() noexcept
    {
        SPDLOG_INFO("GameScene Enter");

        // 카메라 및 라이트 설정
        SetupCameraAndLight();

        // 미로 생성
        CreateLabyrinthBoard();

        // 현재 레벨 로드
        CreateLabyrinthLevel(GameManager::currentLevel);

        // 플레이어 생성
        CreatePlayer();

        // 배경음악 및 효과음 로드
        SetupAudio();
    }

    void OnUpdate() noexcept
    {
        HandleInput();

        UpdateGameLogic();
    }

private:
    // -------------------------------------------------------
    // [초기화 관련 함수들]
    // -------------------------------------------------------
    void SetupCameraAndLight();
    void SetupAudio();
    void CreateLabyrinthBoard();
    void CreateLabyrinthLevel(int levelNum);
    void CreatePlayer();

    // -------------------------------------------------------
    // [업데이트 관련 함수들]
    // -------------------------------------------------------
    void HandleInput();
    void UpdateGameLogic();
    void UpdatePhysicsWalls();
    void CreateCube(Object* parent, Mesh* mesh, Texture* texture, glm::vec3 pos, glm::vec3 scale, bool isWall);

private:
    Camera*           mainCamera       = nullptr;
    Light*            mainLight        = nullptr;

    Object*           boardPivot       = nullptr;
    Object*           xFramePivot      = nullptr;
    Object*           zFramePivot      = nullptr;
    Object*           xHandlePivot     = nullptr;
    Object*           zHandlePivot     = nullptr;
    Object*           playerObject     = nullptr;
    PlayerController* playerController = nullptr;

    std::vector<OBB*> wallOBBs;

    glm::vec3 startPosition;
    glm::vec3 goalPosition;

    float rotatedAmountX = 0.0f;
    float rotatedAmountZ = 0.0f;

    AudioSource* bgmPlayer    = nullptr;
    AudioSource* resurrection = nullptr;
    AudioSource* goalSound    = nullptr;
    AudioSource* ballSound    = nullptr;
    float        checkHitWall = 0.0f;

    Mesh* meshSphere = ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Ball.obj");
    Mesh* meshCube   = ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Cube.obj");

    Texture* texBall   = ResourceManager::LoadResource<Texture>("Assets\\Textures\\Poketball.png");
    Texture* texWood1  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture1.png");
    Texture* texWood2  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture2.png");
    Texture* texWood3  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture3.png");
    Texture* texWood4  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture4.png");
    Texture* texWood5  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\mapBase.png");
    Texture* texWall   = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wall.png");
    Texture* texHandle = ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle.png");
    Texture* texBar    = ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle_bar.png");
    Texture* texRed    = ResourceManager::LoadResource<Texture>("Assets\\Textures\\Red.png");
    Texture* texGreen  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\Green.png");
};