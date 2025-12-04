#pragma once

#include <Framework/Scenes.h>
#include <Framework/Objects.h>
#include <Framework/Audio.h>
#include <Framework/Resources.h>

class TestScene : public Scene
{
public:
	explicit TestScene() noexcept
		: Scene()
        , testObject(nullptr)
	{

	}

	virtual ~TestScene() noexcept override
	{

    }

	virtual void OnEnter() noexcept override
	{
        Object* cameraObj = AddObject("Main Camera", "Camera");
        cameraObj->AddComponent<Camera>();
        cameraObj->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
        
        cameraObj->AddComponent<Camera>();

        cameraObj->AddComponent<AudioListener>();

        testObject = AddObject("Test Object", "Test Tag");
        testObject->GetTransform()->SetPosition(glm::vec3(1.0f, 2.0f, 3.0f));

		auto source = testObject->AddComponent<AudioSource>();
        source->SetClip(ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\Music\\BGM_Boss1.flac"));
        source->Play();
    }

private:
    Object* testObject;
};
