#include "Scene.h"

Scene::Scene(std::string_view name_) noexcept
    : name(name_)
{
}

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