#include "TestScene.h"

#include "../Core/Input.h"
#include "../Core/Time.h"

TestScene::TestScene() noexcept
    : Scene("TestScene")
{
    make_shaderProgram();

    glEnable(GL_DEPTH_TEST);
}

void TestScene::make_vertexShaders()
{
    vertexSource = filetobuf("vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);
    glCompileShader(vertexShader);

    // 에러 체크
    GLint  result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        SPDLOG_INFO("ERROR: vertex shader fail");
        return;
    }
    else
        SPDLOG_INFO("vertex shader sucess");
}

void TestScene::make_fragmentShaders()
{
    fragmentSource = filetobuf("fragment.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // 에러 체크
    GLint  result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        SPDLOG_INFO("ERROR: frag_shader fail");
        return;
    }
    else
        SPDLOG_INFO("fragment shader sucess");
}

void TestScene::make_shaderProgram()
{
    make_vertexShaders();
    make_fragmentShaders();

    shaderProgramID = glCreateProgram();

    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgramID);
}

char* TestScene::filetobuf(const char* file)
{
    FILE* fptr;
    long  length;
    char* buf;
    fopen_s(&fptr, file, "rb");
    if (!fptr)
        return NULL;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf    = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;
    return buf;
}

TestScene::~TestScene() noexcept
{
    delete cam;
    delete cube;
    for (auto& b : map)
        delete b;
    delete light;
}

void TestScene::OnEnter() noexcept
{
    SPDLOG_INFO("Entered TestScene.");

    cam   = new Camera();
    cam->rotate(45.0f);

    cube = new Cube();
    cube->move(Vector3(0.0f, 5.0f, 0.0f));
    cube->resize(glm::vec3(0.5f, 0.5f, 0.5f));

    for (int i{ -7 }; i <= 7; ++i)
    {
        for (int j{-7}; j <= 7; ++j)
        {
            map.push_back(new Cube());
            map.back()->move(glm::vec3(float(i), 0.0f, float(j)));
        }
    }

    for (int i{-7}; i <= 7; ++i)

    light = new Light(Vector3(3.0f, 5.0f, 0.0f));

    labyrinth = new Labyrinth();
}

void TestScene::OnUpdate() noexcept
{
    // 카메라 이동은 스페이스바 누르면서 하게 함
    if (Input::IsKeyHeld(GLFW_KEY_SPACE))
    {
        if (Input::IsKeyHeld(GLFW_KEY_W))
        {
            cam->moveUp();
        }
        else if (Input::IsKeyHeld(GLFW_KEY_S))
        {
            cam->moveDown();
        }

        if (Input::IsKeyHeld(GLFW_KEY_A))
        {
            cam->moveLeft();
        }
        else if (Input::IsKeyHeld(GLFW_KEY_D))
        {
            cam->moveRight();
        }

        if (Input::IsKeyHeld(GLFW_KEY_Q))
        {
            cam->rotate(0.5f);
        }
        else if (Input::IsKeyHeld(GLFW_KEY_E))
        {
            cam->rotate(-0.5f);
        }

        if (Input::IsMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT))
        {
            cam->moveForward();
        }
        else if (Input::IsMouseButtonHeld(GLFW_MOUSE_BUTTON_RIGHT))
        {
            cam->moveBackward();
        }
    }
    else
    {
        if (Input::IsKeyPressed(GLFW_KEY_SPACE))
        {
            cube->teleport(Vector3(0.0f, 5.0f, 0.0f));
        }
        else if (Input::IsKeyPressed(GLFW_KEY_Q))
        {
            cube->rotate(10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        }
        else if (Input::IsKeyPressed(GLFW_KEY_E))
        {
            cube->rotate(-10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        }
        else if (Input::IsKeyPressed(GLFW_KEY_T))
        {
            cube->resize(glm::vec3(0.5f,0.5f,0.5f));
        }
    }

    /*glm::vec2 mousedelta = Input::GetMousePositionDelta();
    if (mousedelta.x > 0)
    {
        labyrinth->Xrotate(0.2f);
    }
    else if (mousedelta.x < 0)
    {
        labyrinth->Xrotate(-0.2f);
    }

    if (mousedelta.y > 0)
    {
        labyrinth->Zrotate(0.2f);
    }
    else if (mousedelta.y < 0)
    {
        labyrinth->Zrotate(-0.2f);
    }*/

    if (Input::IsKeyHeld(GLFW_KEY_J))
    {
        labyrinth->Zrotate(-0.2f);
    }
    else if (Input::IsKeyHeld(GLFW_KEY_L))
    {
        labyrinth->Zrotate(0.2f);
    }
    else if (Input::IsKeyHeld(GLFW_KEY_I))
    {
        labyrinth->Xrotate(0.2f);
    }
    else if (Input::IsKeyHeld(GLFW_KEY_K))
    {
        labyrinth->Xrotate(-0.2f);
    }

    for (int i{}; i < map.size(); ++i)
    {
        cube->checkCollisions(map[i]);
    }
    cube->Update();
}

void TestScene::OnRender() noexcept
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramID);

    cam->settingCamera(shaderProgramID);

    light->applyLight(shaderProgramID);

    cube->Draw(shaderProgramID);

    labyrinth->draw(shaderProgramID);

    for (const auto& m : map)
        m->Draw(shaderProgramID);
}

void TestScene::OnExit() noexcept
{
    SPDLOG_INFO("Exited TestScene.");
}