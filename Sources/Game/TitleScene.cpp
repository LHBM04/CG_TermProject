#include "TitleScene.h"

TitleScene::TitleScene() noexcept
    : Scene()
{
}

TitleScene::~TitleScene() noexcept
{
}

void TitleScene::OnEnter() noexcept
{
    // 카메라
    Object* const cameraObject = AddGameObject("Main Camera", "Camera");
    mainCamera                 = cameraObject->AddComponent<Camera>();
    mainCamera->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));
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
    Object* const lightObject = AddGameObject("Directional Light", "Light");
    lightObject->GetTransform()->SetPosition(glm::fvec3(0.0f, 3.0f, 0.0f));
    lightObject->GetTransform()->LookAt(glm::fvec3(0.0f, 0.0f, 0.0f));
    mainLight = lightObject->AddComponent<Light>();
    mainLight->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));
    mainLight->SetColor(glm::fvec3(1.0f, 1.0f, 1.0f));

    // 배경 미로
    CreateLabyrinthBackground();

    // BGM
    auto bgmClip = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\TitleSceneMusic.mp3");
    bgmPlayer    = AddGameObject("BGM Player", "Audio")->AddComponent<AudioSource>();
    bgmPlayer->SetLooping(true);
    bgmPlayer->SetVolume(0.5f);
    bgmPlayer->SetClip(bgmClip);
    bgmPlayer->Play();


    // 제목 설정
    Object* uiObject = AddUIObject("Title Image", "UI");
    
    uiObject->GetTransform()->SetPosition(
            glm::vec3(Application::GetWindowWidth() * 0.5f, 0.0f, 0.0f));
    uiObject->GetTransform()->SetScale(glm::vec3(600.0f, 200.0f, 1.0f));
    imageTitle = uiObject->AddComponent<ImageRenderer>();

    if (imageTitle)
    {
        imageTitle->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\UIObject"));
        imageTitle->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Rect.obj"));
        imageTitle->SetTexture(ResourceManager::LoadResource<Texture>("Assets\\Textures\\TitleImage.png"));
    }
}

void TitleScene::OnUpdate() noexcept
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

void TitleScene::CreateLabyrinthBackground()
{
    boardPivot   = AddGameObject("BoardPivot", "Pivot");
    xFramePivot  = AddGameObject("XFramePivot", "Pivot");
    zFramePivot  = AddGameObject("ZFramePivot", "Pivot");
    xHandlePivot = AddGameObject("XHandlePivot", "Pivot");
    xHandlePivot->GetTransform()->SetPosition(glm::vec3(10.5f, -3.0f, 0.0f));
    zHandlePivot = AddGameObject("ZHandlePivot", "Pivot");
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

void TitleScene::CreateCube(Object* parent, Mesh* mesh, Texture* texture, glm::vec3 position, glm::vec3 scale)
{
    Object* obj = AddGameObject("Deco", "Deco");
    if (parent)
        obj->GetTransform()->SetParent(parent->GetTransform());
    obj->GetTransform()->SetPosition(position);
    obj->GetTransform()->SetScale(scale);

    MeshRenderer* renderer = obj->AddComponent<MeshRenderer>();
    renderer->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));
    renderer->SetMesh(mesh);
    renderer->SetTexture(texture);

    // OBB 추가 안 함
}

void TitleScene::HandleCameraAnimation()
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

void TitleScene::HandleBoard()
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