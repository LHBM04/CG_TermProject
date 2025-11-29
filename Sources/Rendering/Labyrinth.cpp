#include "Labyrinth.h"

Labyrinth::Labyrinth()
{
    base.push_back(new Cube());
    base[0]->setTexture(wood_texture4);

    Xhandle.reserve(4);
    Xhandle.push_back(new Cube("Sources/Rendering/handle.png"));
    Xhandle.push_back(new Cube("Sources/Rendering/handle_bar.png"));
    Xhandle.push_back(new Cube("Sources/Rendering/handle_bar.png"));
    Xhandle.push_back(new Cube("Sources/Rendering/handle_bar.png"));

    Zhandle.reserve(4);
    Zhandle.push_back(new Cube("Sources/Rendering/handle.png"));
    Zhandle.push_back(new Cube("Sources/Rendering/handle_bar.png")); 
    Zhandle.push_back(new Cube("Sources/Rendering/handle_bar.png"));
    Zhandle.push_back(new Cube("Sources/Rendering/handle_bar.png"));

    XaxisFrame.reserve(4);
    ZaxisFrame.reserve(4);
    for (int i{}; i < 4; ++i)
    {
        XaxisFrame.push_back(new Cube("Sources/Rendering/wood_texture2.png"));
        ZaxisFrame.push_back(new Cube("Sources/Rendering/wood_texture1.png"));
    }

    // 밑바닥 작업
    base[0]->resize(glm::vec3(10.0f, 0.5f, 10.0f));
    base[0]->move(glm::vec3(0.0f, -5.0f, 0.0f));

    // x축 핸들 작업
    Xhandle[0]->resize(glm::vec3(0.5f, 1.0f, 1.0f));
    Xhandle[0]->move(glm::vec3(10.5f, -3.0f, 0.0f));

    Xhandle[1]->resize(glm::vec3(9.4f, 0.2f, 0.2f));
    Xhandle[1]->move(glm::vec3(0.7f, -3.0f, 0.0f));

    Xhandle[2]->resize(glm::vec3(0.2f,0.5f,0.2f));
    Xhandle[3]->resize(glm::vec3(0.2f, 0.5f, 0.2f));

    Xhandle[2]->resize(glm::vec3(0.2f, 1.5f, 0.2f));
    Xhandle[2]->move(glm::vec3(8.5f, -1.5f, 0.0f));
    Xhandle[3]->resize(glm::vec3(0.2f, 1.5f, 0.2f));
    Xhandle[3]->move(glm::vec3(-8.5f, -1.5f, 0.0f));

    // z축 핸들 작업
    Zhandle[0]->resize(glm::vec3(1.0f, 1.0f, 0.5f));
    Zhandle[0]->move(glm::vec3(0.0f, -3.0f, 10.5f));

    Zhandle[1]->resize(glm::vec3(0.2f, 0.2f, 9.7f));
    Zhandle[1]->move(glm::vec3(0.0f, -3.0f, 0.7f));

    Zhandle[2]->resize(glm::vec3(0.2f, 1.5f, 0.2f));
    Zhandle[2]->move(glm::vec3(0.0f, -1.5f, 9.5f));
    Zhandle[3]->resize(glm::vec3(0.2f, 1.5f, 0.2f));
    Zhandle[3]->move(glm::vec3(0.0f, -1.5f, -9.5f));

    // x축 바깥 틀 작업
    XaxisFrame[0]->resize(glm::vec3(0.2f, 1.0f, 8.75f));
    XaxisFrame[0]->move(glm::vec3(8.5f, 0.0f, 0.0f));
    
    XaxisFrame[1]->resize(glm::vec3(0.2f, 1.0f, 8.75f));
    XaxisFrame[1]->move(glm::vec3(-8.5f, 0.0f, 0.0f));

    XaxisFrame[2]->resize(glm::vec3(8.75f, 1.0f, 0.2f));
    XaxisFrame[2]->move(glm::vec3(0.0f, 0.0f, 8.5f));

    XaxisFrame[3]->resize(glm::vec3(8.75f, 1.0f, 0.2f));
    XaxisFrame[3]->move(glm::vec3(0.0f, 0.0f, -8.5f));

    // z축 바깥 틀 작업
    ZaxisFrame[0]->resize(glm::vec3(0.2f, 1.0f, 9.75f));
    ZaxisFrame[0]->move(glm::vec3(9.5f, 0.0f, 0.0f));
    
    ZaxisFrame[1]->resize(glm::vec3(0.2f, 1.0f, 9.75f));
    ZaxisFrame[1]->move(glm::vec3(-9.5f, 0.0f, 0.0f));
    
    ZaxisFrame[2]->resize(glm::vec3(9.75f, 1.0f, 0.2f));
    ZaxisFrame[2]->move(glm::vec3(0.0f, 0.0f, 9.5f));
    
    ZaxisFrame[3]->resize(glm::vec3(9.75f, 1.0f, 0.2f));
    ZaxisFrame[3]->move(glm::vec3(0.0f, 0.0f, -9.5f));
}

void Labyrinth::Xrotate(float theta)
{
    if (rotatedAmountX + theta > maxRotationX)
        return;
    else if (rotatedAmountX + theta < -maxRotationX)
        return;

    Xhandle[0]->rotate(theta, glm::vec3(1.0f, 0.0f, 0.0f));
    for (auto& xf : XaxisFrame)
    {
        xf->rotate(theta, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    }

    rotatedAmountX += theta;
}

void Labyrinth::Zrotate(float theta)
{
    if (rotatedAmountZ + theta > maxRotationZ)
        return;
    else if (rotatedAmountZ + theta < -maxRotationZ)
        return;

    Zhandle[0]->rotate(theta, glm::vec3(0.0f, 0.0f, 1.0f));
    for (auto& zf : ZaxisFrame)
    {
        zf->rotate(theta, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    }

    rotatedAmountZ += theta;
}

void Labyrinth::draw(GLuint shader)
{
    for (const auto& b : base)
    {
        b->Draw(shader);
    }

    for (const auto& xh : Xhandle)
    {
        xh->Draw(shader);
    }
    for (const auto& zh : Zhandle)
    {
        zh->Draw(shader);
    }

    for (auto& xf : XaxisFrame)
    {
        xf->Draw(shader);
    }
    for (auto& zf : ZaxisFrame)
    {
        zf->Draw(shader);
    }
}