#include "Scene.h"

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
}

void Scene::Render() noexcept
{
	OnRender();
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