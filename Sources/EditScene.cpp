#include "EditScene.h"

#include <imgui.h>

#include "Framework/Editor.h"

void EditScene::OnEnter() noexcept
{
}

void EditScene::OnUpdate() noexcept
{
    if (!ImGui::GetIO().WantCaptureMouse)
    {

    }
}

void EditScene::OnFixedUpdate() noexcept
{
}

void EditScene::OnRender() noexcept
{
    Editor::Render();
}

void EditScene::OnExit() noexcept
{
}