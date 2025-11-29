#include "Labyrinth.h"

Labyrinth::Labyrinth()
{
    for (int i{-7}; i <= 7; ++i)
    {
        for (int j{-7}; j <= 7; ++j)
        {
            if (i == -7 || i == 7 || j == -7 || j == 7)
            {
                map.push_back(new Cube());
                map.back()->setTexture(wood_texture3);
                map.back()->move(glm::vec3(float(i), 1.0f, float(j)));
            }
        }
    }
    map.push_back(new Cube());
    map.back()->setTexture(wood_texture3);
    map.back()->resize(glm::vec3(7.5f, 0.0f, 7.5f));

    base.push_back(new Cube());
    base[0]->setTexture(wood_texture4);

    Xhandle.reserve(4);
    Xhandle.push_back(new Cube());
    Xhandle[0]->setTexture(handle);
    Xhandle.push_back(new Cube());
    Xhandle.push_back(new Cube());
    Xhandle.push_back(new Cube());
    for (int i{ 1 }; i < 4; ++i)
    {
        Xhandle[i]->setTexture(handle_bar);
    }

    Zhandle.reserve(4);
    Zhandle.push_back(new Cube());
    Zhandle[0]->setTexture(handle);
    Zhandle.push_back(new Cube()); 
    Zhandle.push_back(new Cube());
    Zhandle.push_back(new Cube());
    for (int i{1}; i < 4; ++i)
    {
        Zhandle[i]->setTexture(handle_bar);
    }

    XaxisFrame.reserve(4);
    ZaxisFrame.reserve(4);
    for (int i{}; i < 4; ++i)
    {
        XaxisFrame.push_back(new Cube());
        XaxisFrame[i]->setTexture(wood_texture2);
        ZaxisFrame.push_back(new Cube());
        ZaxisFrame[i]->setTexture(wood_texture1);
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
    for (auto& m : map)
    {
        m->rotate(theta, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
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
    for (auto& m : map)
    {
        m->rotate(theta, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    }

    rotatedAmountZ += theta;
}

void Labyrinth::draw(GLuint shader)
{
    for (const auto& m : map)
    {
        m->Draw(shader);
    }

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