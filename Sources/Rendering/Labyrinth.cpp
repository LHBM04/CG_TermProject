#include "Labyrinth.h"

Labyrinth::Labyrinth()
{
    base    = new Cube("Sources/Rendering/wood_texture4.png");

    Xhandle[0] = new Cube("Sources/Rendering/handle.png");
    Xhandle[1] = new Cube("Sources/Rendering/handle_bar.png");

    Zhandle[0]  = new Cube("Sources/Rendering/handle.png");
    Zhandle[1] = new Cube("Sources/Rendering/handle_bar.png");
    for (int i{}; i < 4; ++i)
    {
        XaxisFrame.push_back(new Cube("Sources/Rendering/wood_texture2.png"));
        ZaxisFrame.push_back(new Cube("Sources/Rendering/wood_texture1.png"));
    }

    base->resize(glm::vec3(10.0f, 0.5f, 10.0f));
    base->move(glm::vec3(0.0f, -3.5f, 0.0f));

    Xhandle[0]->resize(glm::vec3(0.5f, 1.0f, 1.0f));
    Xhandle[0]->move(glm::vec3(10.5f, 0.0f, 0.0f));

    Xhandle[1]->resize(glm::vec3(9.7f, 0.2f, 0.2f));
    Xhandle[1]->move(glm::vec3(0.0f, -1.0f, 0.0f));

    Zhandle[0]->resize(glm::vec3(1.0f, 1.0f, 0.5f));
    Zhandle[0]->move(glm::vec3(0.0f, 0.0f, 10.5f));

    Zhandle[1]->resize(glm::vec3(0.2f, 0.2f, 8.7f));
    Zhandle[1]->move(glm::vec3(0.0f, -1.0f, 0.0f));

    XaxisFrame[0]->resize(glm::vec3(0.2f, 1.0f, 8.75f));
    XaxisFrame[0]->move(glm::vec3(8.5f, 0.0f, 0.0f));
    
    XaxisFrame[1]->resize(glm::vec3(0.2f, 1.0f, 8.75f));
    XaxisFrame[1]->move(glm::vec3(-8.5f, 0.0f, 0.0f));

    XaxisFrame[2]->resize(glm::vec3(8.75f, 1.0f, 0.2f));
    XaxisFrame[2]->move(glm::vec3(0.0f, 0.0f, 8.5f));

    XaxisFrame[3]->resize(glm::vec3(8.75f, 1.0f, 0.2f));
    XaxisFrame[3]->move(glm::vec3(0.0f, 0.0f, -8.5f));

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
}

void Labyrinth::Zrotate(float theta)
{
}

void Labyrinth::draw(GLuint shader)
{
    base->Draw(shader);
    for (int i{}; i < 2; ++i)
    {
        Xhandle[i]->Draw(shader);
        Zhandle[i]->Draw(shader);
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