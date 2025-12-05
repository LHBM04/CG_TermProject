#pragma once

#include <vector>
#include "../Framework/Application.h"
#include "../Framework/Audio.h"
#include "../Framework/Input.h"
#include "../Framework/Rendering.h"
#include "../Framework/Resources.h"
#include "../Framework/Scenes.h"
#include "../Framework/Time.h"
#include "../Framework/UI.h"

#include "Spline.h"

/**
 * @class TitleScene
 *
 * @brief Labyrinth 게임 씬
 */
class TitleScene final : public Scene
{
public:
    explicit TitleScene() noexcept;
    virtual ~TitleScene() noexcept override;

    virtual void OnEnter() noexcept override;
    virtual void OnUpdate() noexcept override;

private:
    void CreateLabyrinthBackground();
    void CreateCube(Object* parent, Mesh* mesh, Texture* texture, glm::vec3 position, glm::vec3 scale);
    void HandleCameraAnimation();
    void HandleBoard();

private:
    Camera* mainCamera   = nullptr;
    Spline* cameraSpline = nullptr;

    Light* mainLight = nullptr;

    // 메인화면 브금
    AudioSource* bgmPlayer = nullptr;

    // 회전 중심점들
    Object* boardPivot   = nullptr; // 미로 바닥 + 벽
    Object* xFramePivot  = nullptr; // X축 프레임
    Object* zFramePivot  = nullptr; // Z축 프레임
    Object* xHandlePivot = nullptr; // X축 손잡이의 피봇
    Object* zHandlePivot = nullptr; // Z축 손잡이의 피봇

    // 회전 상태
    float       rotatedAmountX = 0.0f;
    float       rotatedAmountZ = 0.0f;
    const float maxRotation    = 10.0f;


    // 텍스쳐를 실행 도중에 로드하면 렉 걸려서 처음 실행될 때 미리 다 로드하게 함
    Mesh* meshCube = ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Cube.obj");

    Texture* wall      = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wall.png");
    Texture* texWood1  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture1.png");
    Texture* texWood2  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture2.png");
    Texture* texWood3  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture3.png");
    Texture* texWood4  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\wood_texture4.png");
    Texture* texWood5  = ResourceManager::LoadResource<Texture>("Assets\\Textures\\mapBase.png");
    Texture* texHandle = ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle.png");
    Texture* texBar    = ResourceManager::LoadResource<Texture>("Assets\\Textures\\handle_bar.png");

    Texture* texGreen = ResourceManager::LoadResource<Texture>("Assets\\Textures\\Green.png");
    Texture* texRed   = ResourceManager::LoadResource<Texture>("Assets\\Textures\\Red.png");
};