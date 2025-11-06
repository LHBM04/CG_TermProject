#include "TestScene.h"

#include "../Core/Input.h"
#include "../Core/Time.h"

TestScene::TestScene() noexcept
    : Scene("TestScene")
    , testObject(gluNewQuadric())
{
   
}

TestScene::~TestScene() noexcept
{
    gluDeleteQuadric(testObject);
}

void TestScene::OnEnter() noexcept
{
    SPDLOG_INFO("Entered TestScene.");
}

void TestScene::OnUpdate() noexcept
{
    if (Input::IsKeyPressed(GLFW_KEY_SPACE))
    {
        SPDLOG_INFO("Space key pressed in TestScene.");
    }
}

void TestScene::OnRender() noexcept
{
    gluSphere(testObject, 1.0f, 32, 32);
}

void TestScene::OnExit() noexcept
{
    SPDLOG_INFO("Exited TestScene.");
}