#include "GameScene.h"
#include "../Framework/Debug.h" // SPDLOG_INFO 등을 사용하기 위해 필요

using json = nlohmann::json;

GameScene::GameScene() noexcept
    : Scene()
{
}

GameScene::~GameScene() noexcept
{
}

void GameScene::InitializeVariables()
{
    wallOBBs.clear();

    isGoalReached  = false;
    goalDelayTimer = 0.0f;

    rotatedAmountX = 0.0f;
    rotatedAmountZ = 0.0f;
    checkHitWall   = 0.0f;

    goalImage      = nullptr;
    spectatorImage = nullptr;
}

void GameScene::SetupCameraAndLight()
{
    Object* cameraObj = AddGameObject("Main Camera", "Camera");
    cameraObj->GetTransform()->SetPosition(glm::vec3(0.0f, 20.0f, 5.0f));
    cameraObj->GetTransform()->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    mainCamera = cameraObj->AddComponent<Camera>();
    mainCamera->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));

    Object* lightObj = AddGameObject("Directional Light", "Light");
    lightObj->GetTransform()->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    lightObj->GetTransform()->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    mainLight = lightObj->AddComponent<Light>();
    if (mainLight)
    {
        mainLight->SetColor(glm::vec3(1.0f));
        mainLight->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));
    }
}

void GameScene::SetupAudio()
{
    auto goalClip = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\goal.wav");
    goalSound     = AddGameObject("Goal Sound", "SFX")->AddComponent<AudioSource>();
    goalSound->SetClip(goalClip);

    auto resClip = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\resurrection.wav");
    resurrection = AddGameObject("Resurrection", "SFX")->AddComponent<AudioSource>();
    resurrection->SetVolume(0.5f);
    resurrection->SetClip(resClip);

    auto hitWallClip = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\hitWall.wav");
    ballSound        = AddGameObject("hitWall Sound", "SFX")->AddComponent<AudioSource>();
    ballSound->SetClip(hitWallClip);

    // BGM
    auto bgmClip = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\Stickerbush Symphony Restored to HD.mp3");
    bgmPlayer    = AddGameObject("BGM Player", "Audio")->AddComponent<AudioSource>();
    bgmPlayer->SetLooping(true);
    bgmPlayer->SetVolume(0.5f);
    bgmPlayer->SetClip(bgmClip);
    bgmPlayer->Play();
}

void GameScene::SetupFont()
{
    Font* conversationFont = ResourceManager::LoadResource<Font>("Assets\\Fonts\\Conversation.ttf");

    // 클리어 시간
    Object* timerViewObj = AddUIObject("Timer View", "UI");
    timerViewObj->GetTransform()->SetPosition(glm::vec3(0.0f, 50.0f, 0.0f));
    timerViewObj->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

    timerView = timerViewObj->AddComponent<TextRenderer>();
    timerView->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Text"));
    timerView->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Rect.obj"));
    timerView->SetFont(conversationFont);

    // 죽은 횟수
    Object* deathCountViewObj = AddUIObject("Death Count View", "UI");
    deathCountViewObj->GetTransform()->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
    deathCountViewObj->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

    deathCountView = deathCountViewObj->AddComponent<TextRenderer>();
    deathCountView->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Text"));
    deathCountView->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Rect.obj"));
    deathCountView->SetFont(conversationFont);

    // 대화
    Object* conversationViewObj = AddUIObject("Conversation View", "UI");
    conversationViewObj->GetTransform()->SetPosition(
            glm::vec3(Application::GetWindowWidth() * 1.0f, Application::GetWindowHeight() * 1.0f, 0.0f));
    conversationViewObj->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

    conversationView = conversationViewObj->AddComponent<TextRenderer>();
    conversationView->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Text"));
    conversationView->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Rect.obj"));
    conversationView->SetFont(conversationFont);

    switch (GameManager::currentLevel)
    {
        case 0:
            conversation = "몸 풀기 단계야. 준비 됐어?";
            break;
        case 1:
            conversation = "넌 할 수 있어";
            break;
        case 2:
            conversation = "생각보다 어렵지?";
            break;
        case 3:
            conversation = "이게 뭐야?";
            break;
        case 4:
            conversation = "난이도를 높여볼게. 기대해도 좋아";
            break;
        case 5:
            conversation = "포기하는게 어때?";
            break;
        case 6:
            conversation = "넌 할 수 없어";
            break;
        case 7:
            conversation = "...";
            break;
        default:
            conversation = "에러";
            break;
    }
}

void GameScene::CreateLabyrinthBoard()
{
    // 피봇 생성
    boardPivot   = AddGameObject("BoardPivot", "Pivot");
    xFramePivot  = AddGameObject("XFramePivot", "Pivot");
    zFramePivot  = AddGameObject("ZFramePivot", "Pivot");
    xHandlePivot = AddGameObject("XHandlePivot", "Pivot");
    xHandlePivot->GetTransform()->SetPosition(glm::vec3(10.5f, -3.0f, 0.0f));
    zHandlePivot = AddGameObject("ZHandlePivot", "Pivot");
    zHandlePivot->GetTransform()->SetPosition(glm::vec3(0.0f, -3.0f, 10.5f));

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

    CreateCube(boardPivot, meshCube, texWood5, glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(15.0f, 1.0f, 15.0f), false);
    CreateCube(nullptr, meshCube, texWood4, glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f), false);
}

void GameScene::CreateLabyrinthLevel(int levelNum)
{
    std::string   path = "Assets/Map/level" + std::to_string(levelNum) + ".json";
    std::ifstream file(path);
    if (!file.is_open())
        return;

    json data;
    file >> data;
    file.close();

    int              width   = data["width"];
    int              height  = data["height"];
    std::vector<int> tiles   = data["tiles"];
    float            offsetX = width / 2.0f;
    float            offsetZ = height / 2.0f;

    for (int z = 0; z < height; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            int type = tiles[z * width + x];
            if (type == 0)
                continue;

            float posX = (float)x - offsetX + 0.5f;
            float posZ = (float)z - offsetZ + 0.5f;

            // 바닥 생성
            CreateCube(boardPivot, meshCube, texWood3, glm::vec3(posX, -0.5f, posZ), glm::vec3(1.0f), true);

            // 벽 생성
            if (type == 2)
            {
                CreateCube(boardPivot, meshCube, texWall, glm::vec3(posX, 0.5f, posZ), glm::vec3(1.0f), true);
            }
            // 시작 지점
            else if (type == 3)
            {
                startPosition = glm::vec3(posX, 2.0f, posZ);
                CreateCube(
                        boardPivot, meshCube, texRed, glm::vec3(posX, -0.5f, posZ), glm::vec3(1.0f, 1.2f, 1.0f), false);
            }
            // 골인 지점
            else if (type == 4)
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

    Object* tmp = AddUIObject("Spectator Image", "UI");
    tmp->GetTransform()->SetScale(glm::vec3(600.0f, 300.0f, 1.0f));
    spectatorImage = tmp->AddComponent<ImageRenderer>();
    spectatorImage->GetTransform()->SetPosition(glm::vec3(200.0f, Application::GetWindowHeight() - 100.0f, 0.0f));
    spectatorImage->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\UIObject"));
    spectatorImage->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Rect.obj"));
    spectatorImage->SetTexture(ResourceManager::LoadResource<Texture>(
            std::string("Assets\\Textures\\level" + std::to_string(GameManager::currentLevel % 4)  + "Text.png")));
}

void GameScene::CreatePlayer()
{
    playerObject = AddGameObject("Player", "Player");
    playerObject->GetTransform()->SetPosition(startPosition);
    playerObject->GetTransform()->SetScale(glm::vec3(0.7f));

    playerObject->AddComponent<MeshRenderer>()->SetMesh(meshSphere);
    playerObject->GetComponent<MeshRenderer>()->SetTexture(texBall);
    playerObject->GetComponent<MeshRenderer>()->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));

    OBB* obb = playerObject->AddComponent<OBB>();
    obb->resize(glm::vec3(0.35f)); // 반지름
    obb->teleport(startPosition);

    playerController = playerObject->AddComponent<PlayerController>();
    playerController->SetGravityScale(1.0f);
    playerController->SetSlopeBoost(2.5f);
}

void GameScene::HandleInput()
{
    if (isGoalReached)
        return;

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


    // R키 리셋
    if (InputManager::IsKeyPressed(Keyboard::R))
    {
        playerObject->GetTransform()->SetPosition(startPosition);
        playerController->setDir(glm::vec3(0));
        resurrection->Play();
    }
}

void GameScene::ChangeFontValue()
{
    if (deathCountView)
        deathCountView->SetText(std::format("DEATH: {}", GameManager::deathCount - 1));

    if (timerView)
        timerView->SetText(std::format("{:.2f}", GameManager::playTime));

    if (conversationView)
    {
        conversationView->SetText(std::format("{}", conversation));
    }
}

void GameScene::UpdateGameLogic()
{
    if (!playerController || !boardPivot)
        return;

    // 물리 벽 위치 동기화
    UpdatePhysicsWalls();

    // 바닥 기울기 전달
    glm::vec3 normal = boardPivot->GetTransform()->GetUp();
    playerController->SetGroundNormal(normal);

    // 충돌 체크
    for (OBB* wall : wallOBBs)
    {
        playerController->CheckCollision(wall);
    }

    // 낙하 체크
    if (playerObject->GetTransform()->GetPosition().y < -10.0f)
    {
        playerObject->GetTransform()->SetPosition(startPosition);
        playerController->setDir(glm::vec3(0));
        GameManager::deathCount++;
        resurrection->Play();
    }

    // 골인 체크
    glm::vec3 pPos = playerObject->GetTransform()->GetPosition();
    if (glm::distance(pPos, goalPosition) < 1.1f && !isGoalReached)
    {
        SPDLOG_INFO("goal in..!");
        isGoalReached = true;
        goalSound->Play();

        GameManager::NextLevel();

        Object* goalObj = AddUIObject("Goal Image", "UI");
        goalObj->GetTransform()->SetScale(glm::vec3(600.0f, 300.0f, 1.0f));
        goalImage = goalObj->AddComponent<ImageRenderer>();
        goalImage->GetTransform()->SetPosition(glm::vec3(Application::GetWindowWidth() * 0.5f, Application::GetWindowHeight() * 0.5f, 0.0f));
        goalImage->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\UIObject"));
        goalImage->SetMesh(ResourceManager::LoadResource<Mesh>("Assets\\Meshes\\Rect.obj"));
        goalImage->SetTexture(ResourceManager::LoadResource<Texture>("Assets\\Textures\\Congratulations.png"));
    }
    else
    {
        GameManager::playTime += TimeManager::GetDeltaTime();
    }

    // 골인하면 효과음 재생할 시간정도만 딜레이 후 다음 레벨 진입
    if (isGoalReached)
    {
        goalDelayTimer += TimeManager::GetDeltaTime();

        if (goalDelayTimer >= 3.0f)
        {
            if (GameManager::currentLevel <= GameManager::maxLevel)
            {
                std::ifstream file(
                        std::string("Assets/Map/level" + std::to_string(GameManager::currentLevel) + ".json"));
                file.close();
                SceneManager::LoadScene("Game Scene");
                return;
            }
            else
            {
                SceneManager::LoadScene("Title Scene");
                return;
            }
        }
    }

    // 소리 재생
    float slidingSoundVolume = glm::clamp(glm::length(playerController->GetDir()) * 0.14f, 0.0f, 1.0f);
    float hitVolume          = abs(slidingSoundVolume - checkHitWall);

    if (hitVolume > 0.1f)
    {
        ballSound->SetVolume(hitVolume * 2.0f);
        ballSound->SetPitch(hitVolume);
        ballSound->Play();
        checkHitWall = slidingSoundVolume;
    }
}

void GameScene::UpdatePhysicsWalls()
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

void GameScene::CreateCube(Object* parent, Mesh* mesh, Texture* texture, glm::vec3 pos, glm::vec3 scale, bool isWall)
{
    Object* obj = AddGameObject("Cube", isWall ? "Wall" : "Deco");
    if (parent)
        obj->GetTransform()->SetParent(parent->GetTransform());
    obj->GetTransform()->SetPosition(pos);
    obj->GetTransform()->SetScale(scale);

    auto render = obj->AddComponent<MeshRenderer>();
    render->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));
    render->SetMesh(mesh);
    render->SetTexture(texture);

    if (isWall)
    {
        OBB* obb = obj->AddComponent<OBB>();
        obb->resize(scale * 0.5f);
        obb->teleport(pos);
        wallOBBs.push_back(obb);
    }
}