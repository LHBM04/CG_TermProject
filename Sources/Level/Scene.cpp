#include "Scene.h"

#include "../Rendering/Camera.h"

Scene::~Scene() noexcept
{
}

void Scene::Enter() noexcept
{
	OnEnter();
}

void Scene::Update() noexcept
{
	OnUpdate();

	for (const auto& entity : entities.view<>())
	{
		// Update entity
	}

	for (const auto& interface : interfaces.view<>())
	{
		// Update interface
	}
}

void Scene::Render() noexcept
{
    for (const std::unique_ptr<Camera>& camera : cameras)
	{
		camera->Ready();

		for (const auto& entity : entities.view<>())
		{
		// Render entity
		}

		for (const auto& interface : interfaces.view<>())
        {
        }
	}
}

void Scene::Exit() noexcept
{
	OnExit();
}

void Scene::OnEnter() noexcept
{
}

void Scene::OnUpdate() noexcept
{
}

void Scene::OnRender() noexcept
{
}

void Scene::OnExit() noexcept
{
}