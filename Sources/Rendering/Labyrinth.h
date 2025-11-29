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
    // 중앙의 맵
    std::vector<Cube*> map;

    // x축 회전을 보여주는 외부 틀
    std::vector<Cube*> XaxisFrame;

    // z축 회전을 보여주는 외부 틀
    std::vector<Cube*> ZaxisFrame;

    // 회전에 따라 돌아가는 손잡이 ( 나중에 육각형말고 원통으로 바꿔야 더 좋을 듯 )
    std::vector<Cube*> Xhandle;
    std::vector<Cube*> Zhandle;

    // 바닥
    std::vector<Cube*> base;

    // 일정 회전 이상은 금지용
    float rotatedAmountX = 0.0f;
    float maxRotationX = 10.0f;

    float rotatedAmountZ = 0.0f;
    float maxRotationZ = 10.0f;

    // 텍스쳐 매핑 이미지
    GLsizei width, height, numberOfChannel;
    GLuint  wood_texture1 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture1.png", width, height, numberOfChannel);
    GLuint  wood_texture2 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture2.png", width, height, numberOfChannel);
    GLuint  wood_texture3 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture3.png", width, height, numberOfChannel);
    GLuint  wood_texture4 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture4.png", width, height, numberOfChannel);
    GLuint  handle        = TextureLoader::LoadTexture("Sources/Rendering/handle.png", width, height, numberOfChannel);
    GLuint  handle_bar    = TextureLoader::LoadTexture("Sources/Rendering/handle_bar.png", width, height, numberOfChannel);
};