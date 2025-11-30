#pragma once

#include "../Framework/Application.h"
#include "../Framework/Rendering.h"
#include "../Framework/Input.h"
#include "../Framework/Resources.h"
#include "../Framework/Scenes.h"

/**
 * @class TitleScene
 *
 * @brief 타이틀 씬을 정의합니다.
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
        testCamera = AddObject("Test Camera", "Main Camera")->AddComponent<Camera>();

        // 임시: Scene의 objects 리스트에 접근할 수 없으므로,
        // Scene 생성자 코드를 수정하거나, 여기서 안전하게 새로 만드는 것을 추천합니다.
        // 하지만 이미 있다고 하시니, 'Test Object'를 만들기 전에 카메라를 세팅해 봅니다.

        // ★ 가장 확실한 방법: 여기서 카메라를 새로 만들거나 기존 것을 확실히 세팅 ★

        // 큐브 생성
        Object* testObject = AddObject("Test Object", "Object");
        testObject->GetTransform()->SetPosition(glm::fvec3(0.0f, 0.0f, 0.0f)); // 큐브를 정중앙(0,0,0)에 배치
        testObject->GetTransform()->SetRotation(glm::fvec3(0.0f, 45.0f, 0.0f));

        Mesh* testMesh = ResourceManager::LoadResource<Mesh>("D:/Game Projects/CG_TermProject/Assets/Meshes/Cube.obj");
        MeshRenderer* renderer = testObject->AddComponent<MeshRenderer>();
        renderer->SetMesh(testMesh);

        // [중요] 카메라를 생성하고 위치/회전을 잡습니다.
        // 만약 Scene 생성자에 이미 있다면, 그 오브젝트를 찾아서 수정해야 합니다.
        // 여기서는 덮어쓰거나 새로 추가하여 확실하게 보이게 만듭니다.
        Object* cam = AddObject("MyCamera", "Camera");
        cam->AddComponent<Camera>();

        // 위치: 뒤로 5만큼 (0, 0, 5)
        cam->GetTransform()->SetPosition(glm::fvec3(0.0f, 0.0f, 5.0f));

        // 회전: Y축으로 -90도 돌려야 -Z(앞)를 바라봄
        // (Rendering.h의 수식상 Yaw=-90이어야 front.z가 -1이 됨)
        cam->GetTransform()->SetRotation(glm::fvec3(0.0f, -90.0f, 0.0f));
    }

    virtual void OnUpdate() noexcept override
    {
        if (InputManager::IsKeyPressed(Keyboard::Enter))
        {
            
        }
    }

private:
    Camera* testCamera = nullptr;
};