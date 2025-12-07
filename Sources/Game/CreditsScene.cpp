#include "CreditsScene.h"

#include "Framework/Input.h"
#include "Framework/Objects.h"
#include "Framework/Rendering.h"
#include "Framework/Resources.h"
#include "Framework/UI.h"

void CreditsScene::OnEnter() noexcept
{
    Camera* const camera = AddGameObject("Main Camera", "Camera")->AddComponent<Camera>();
    camera->SetShader(ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard"));

	ImageRenderer* backgroundSR = AddUIObject("Background", "Background")->AddComponent<ImageRenderer>();

    backgroundSR->GetTransform()->SetPosition(glm::vec3(1280 / 2, 720 / 2, 0));
    backgroundSR->GetTransform()->SetScale(glm::vec3(1280, 720, 0));

    backgroundSR->SetShader(ResourceManager::LoadResource<Shader>("Assets/Shaders/UIObject"));
    backgroundSR->SetMesh(ResourceManager::LoadResource<Mesh>("Assets/Meshes/Rect.obj"));
    backgroundSR->SetTexture(ResourceManager::LoadResource<Texture>("Assets/Textures/Credits.png"));
}

void CreditsScene::OnUpdate() noexcept
{
	if (InputManager::IsKeyPressed(Keyboard::Escape))
	{
		SceneManager::LoadScene("Title Scene");
    }
}