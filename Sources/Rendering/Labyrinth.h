#pragma once

#include "../PCH.h"
#include "../Rendering/TextureLoader.h"
#include "Cube.h"

class Labyrinth
{
public:
    Labyrinth();

    void Xrotate(float theta);
    void Zrotate(float theta);

    std::vector<Cube*>& getMap()
    {
        return map;
    }

    void draw(GLuint shader);

private:
    std::vector<Cube*> map;
    std::vector<Cube*> XaxisFrame;
    std::vector<Cube*> ZaxisFrame;
    std::vector<Cube*> Xhandle;
    std::vector<Cube*> Zhandle;
    std::vector<Cube*> base;

    // 맵의 누적 회전 제거용
    std::vector<glm::vec3> initialMapPos;
    std::vector<glm::quat> initialMapRot;

    float rotatedAmountX = 0.0f;
    float maxRotationX = 10.0f;

    float rotatedAmountZ = 0.0f;
    float maxRotationZ = 10.0f;

    glm::quat labyrinthRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 pivot = glm::vec3(0.0f, 0.0f, 0.0f);

    GLsizei width, height, numberOfChannel;
    GLuint  wood_texture1 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture1.png", width, height, numberOfChannel);
    GLuint  wood_texture2 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture2.png", width, height, numberOfChannel);
    GLuint  wood_texture3 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture3.png", width, height, numberOfChannel);
    GLuint  wood_texture4 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture4.png", width, height, numberOfChannel);
    GLuint  handle        = TextureLoader::LoadTexture("Sources/Rendering/handle.png", width, height, numberOfChannel);
    GLuint  handle_bar    = TextureLoader::LoadTexture("Sources/Rendering/handle_bar.png", width, height, numberOfChannel);
};