#pragma once

#include "../Framework/Application.h"
#include "../Framework/Audio.h"
#include "../Framework/Input.h"
#include "../Framework/Resources.h"
#include "../Framework/UI.h"
#include "../Framework/Scenes.h"
#include "../Framework/Time.h"
#include "../Framework/Rendering.h"
#include "GameManager.h"
#include "TitleScene.h"

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

        InputManager::DisableCursor();

        // 변수 초기화
        InitializeVariables();

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

        
    // 제목 설정
        Font* testFont = ResourceManager::LoadResource<Font>("Assets\\Fonts\\Paperlogy-1Thin.ttf");

        Object* timerViewObj = AddUIObject("Timer View", "UI");
        timerViewObj->GetTransform()->SetPosition(glm::vec3(100.0f, 200.0f, 0.0f));
        timerViewObj->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
        
        timerView = timerViewObj->AddComponent<TextRenderer>();
        timerView->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Text"));
        timerView->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Rect.obj"));
        timerView->SetFont(testFont);

        Object* deathCountViewObj = AddUIObject("Death Count View", "UI");
        deathCountViewObj->GetTransform()->SetPosition(glm::vec3(100.0f, 500.0f, 0.0f));
        deathCountViewObj->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
        
        deathCountView = deathCountViewObj->AddComponent<TextRenderer>();
        deathCountView->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Text"));
        deathCountView->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Rect.obj"));
        deathCountView->SetFont(testFont);
    }

    TextRenderer* deathCountView;
    TextRenderer* timerView;

    void OnUpdate() noexcept
    {
        if (deathCountView)
            deathCountView->SetText(std::format("{}", GameManager::deathCount));

        if (timerView)
            timerView->SetText(std::format("{:.2f}", GameManager::playTime));

        mainCamera->GetOwner()->GetComponent<Camera>()->GetTransform()->SetPosition(glm::vec3(0.0f, 20.0f, 5.0f));
        mainCamera->GetOwner()->GetComponent<Camera>()->GetTransform()->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

        HandleInput();

        UpdateGameLogic();
    }

private:
    // -------------------------------------------------------
    // [초기화 관련 함수들]
    // -------------------------------------------------------
    void InitializeVariables();
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

    ImageRenderer* spectatorImage = nullptr;

    glm::vec3 startPosition;
    glm::vec3 goalPosition;
    ImageRenderer* goalImage = nullptr;
    bool      isGoalReached;
    float     goalDelayTimer; // 골인 시 효과음 들릴 시간을 좀 주기 위한 용도

    float rotatedAmountX;
    float rotatedAmountZ;

    AudioSource* bgmPlayer    = nullptr;
    AudioSource* resurrection = nullptr;
    AudioSource* goalSound    = nullptr;
    AudioSource* ballSound    = nullptr;
    float        checkHitWall;

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