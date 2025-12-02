#include "EditScene.h"

#include "Framework/Common.h"
#include "Framework/Application.h"

#include "Editor.h"

EditScene::EditScene() noexcept
{
}

EditScene::~EditScene() noexcept
{
}

void EditScene::OnEnter() noexcept
{
    Editor::Initialize();
}

void EditScene::OnRender() noexcept
{
    Editor::RenderGUI();
}

void EditScene::OnExit() noexcept
{
    Editor::Quit();
}