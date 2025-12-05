#pragma once

#include "../Framework/Application.h"
#include "../Framework/Rendering.h"
#include "../Framework/Input.h"
#include "../Framework/Resources.h"
#include "../Framework/Scenes.h"
#include "../Framework/Time.h"
#include "../Framework/Audio.h"
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
        // 카메라
        Object* const cameraObject = AddObject("Main Camera", "Camera");
        mainCamera                 = cameraObject->AddComponent<Camera>();
        cameraSpline               = cameraObject->AddComponent<Spline>();

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

        // 조명
        Object* const lightObject = AddObject("Directional Light", "Light");
        lightObject->GetTransform()->SetPosition(glm::fvec3(0.0f, 3.0f, 0.0f));
        lightObject->GetTransform()->LookAt(glm::fvec3(0.0f, 0.0f, 0.0f));
        mainLight = lightObject->AddComponent<Light>();
        mainLight->SetColor(glm::fvec3(1.0f, 1.0f, 1.0f));

        // 배경 미로
        CreateLabyrinthBackground();

        // BGM
        auto bgmClip =
                ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\TitleSceneMusic.mp3");
        bgmPlayer = AddObject("BGM Player", "Audio")->AddComponent<AudioSource>();
        bgmPlayer->SetLooping(true);
        bgmPlayer->SetVolume(0.5f);
        bgmPlayer->SetClip(bgmClip);
        bgmPlayer->Play();
    }

    virtual void OnUpdate() noexcept override
    {
        // 카메라 연출
        mainCamera->GetTransform()->SetPosition(cameraSpline->GetTransform()->GetPosition());
        mainCamera->GetTransform()->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

        if (InputManager::IsKeyPressed(Keyboard::Enter))
        {
            SceneManager::LoadScene("Game Scene");
            return;
        }

        // 카메라 이동 애니메이션
        HandleCameraAnimation();

        HandleBoard();
    }

private:
    void CreateLabyrinthBackground()
    {
        boardPivot   = AddObject("BoardPivot", "Pivot");
        xFramePivot  = AddObject("XFramePivot", "Pivot");
        zFramePivot  = AddObject("ZFramePivot", "Pivot");
        xHandlePivot = AddObject("XHandlePivot", "Pivot");
        xHandlePivot->GetTransform()->SetPosition(glm::vec3(10.5f, -3.0f, 0.0f));
        zHandlePivot = AddObject("ZHandlePivot", "Pivot");
        zHandlePivot->GetTransform()->SetPosition(glm::vec3(0.0f, -3.0f, 10.5f));

        CreateCube(boardPivot, meshCube, texWood5, glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(15.0f, 1.0f, 15.0f));
        CreateCube(nullptr, meshCube, texWood4, glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f));

        // X축 핸들
        CreateCube(xHandlePivot, meshCube, texHandle, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 2.0f, 2.0f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.7f, -3.0f, 0.0f), glm::vec3(18.8f, 0.4f, 0.4f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(8.5f, -1.5f, 0.0f), glm::vec3(0.4f, 3.0f, 0.4f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(-8.5f, -1.5f, 0.0f), glm::vec3(0.4f, 3.0f, 0.4f));

        // X축 프레임
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(8.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 17.5f));
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(-8.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 17.5f));
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(0.0f, 0.0f, 8.5f), glm::vec3(17.5f, 2.0f, 0.4f));
        CreateCube(xFramePivot, meshCube, texWood2, glm::vec3(0.0f, 0.0f, -8.5f), glm::vec3(17.5f, 2.0f, 0.4f));

        // Z축 핸들
        CreateCube(zHandlePivot, meshCube, texHandle, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 1.0f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -3.0f, 0.7f), glm::vec3(0.4f, 0.4f, 19.4f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -1.5f, 9.5f), glm::vec3(0.4f, 3.0f, 0.4f));
        CreateCube(nullptr, meshCube, texBar, glm::vec3(0.0f, -1.5f, -9.5f), glm::vec3(0.4f, 3.0f, 0.4f));

        // Z축 프레임
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(9.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 19.5f));
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(-9.5f, 0.0f, 0.0f), glm::vec3(0.4f, 2.0f, 19.5f));
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(0.0f, 0.0f, 9.5f), glm::vec3(19.5f, 2.0f, 0.4f));
        CreateCube(zFramePivot, meshCube, texWood1, glm::vec3(0.0f, 0.0f, -9.5f), glm::vec3(19.5f, 2.0f, 0.4f));
    }

    void CreateCube(Object* parent, Mesh* mesh, Texture* texture, glm::vec3 position, glm::vec3 scale)
    {
        Object* obj = AddObject("Deco", "Deco");
        if (parent)
            obj->GetTransform()->SetParent(parent->GetTransform());
        obj->GetTransform()->SetPosition(position);
        obj->GetTransform()->SetScale(scale);

        MeshRenderer* renderer = obj->AddComponent<MeshRenderer>();
        renderer->SetMesh(mesh);
        renderer->SetTexture(texture);

        // OBB 추가 안 함
    }

    void HandleCameraAnimation()
    {
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
    }

    void HandleBoard()
    {
        float     dt          = TimeManager::GetDeltaTime();
        float     rotateSpeed = 50.0f;
        glm::vec2 mouseDelta  = InputManager::GetMousePositionDelta();

        if (mouseDelta.y != 0)
            rotatedAmountX += (mouseDelta.y > 0 ? 1 : -1) * rotateSpeed * dt;
        if (mouseDelta.x != 0)
            rotatedAmountZ += (mouseDelta.x < 0 ? 1 : -1) * rotateSpeed * dt;

        float maxRotation = 10.0f;
        rotatedAmountX    = glm::clamp(rotatedAmountX, -maxRotation, maxRotation);
        rotatedAmountZ    = glm::clamp(rotatedAmountZ, -maxRotation, maxRotation);

        if (boardPivot)
            boardPivot->GetTransform()->SetRotation(glm::vec3(rotatedAmountX, 0.0f, rotatedAmountZ));

        if (xFramePivot)
            xFramePivot->GetTransform()->SetRotation(glm::vec3(rotatedAmountX, 0.0f, 0.0f));
        if (xHandlePivot)
            xHandlePivot->GetTransform()->SetRotation(glm::vec3(rotatedAmountX, 0.0f, 0.0f));
        if (zFramePivot)
            zFramePivot->GetTransform()->SetRotation(glm::vec3(0.0f, 0.0f, rotatedAmountZ));
        if (zHandlePivot)
            zHandlePivot->GetTransform()->SetRotation(glm::vec3(0.0f, 0.0f, rotatedAmountZ));
    }
private:
    Camera* mainCamera = nullptr;
    Spline* cameraSpline = nullptr;

    Light*  mainLight  = nullptr;

    // 메인화면 브금
    AudioSource* bgmPlayer = nullptr;

    // 회전 중심점들
    Object* boardPivot  = nullptr; // 미로 바닥 + 벽
    Object* xFramePivot = nullptr; // X축 프레임
    Object* zFramePivot = nullptr; // Z축 프레임
    Object* xHandlePivot = nullptr; // X축 손잡이의 피봇
    Object* zHandlePivot = nullptr; // Z축 손잡이의 피봇

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
};