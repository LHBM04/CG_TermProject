#pragma once

#include "../PCH.h"
#include "Cube.h"

class Labyrinth
{
public:
    Labyrinth();

    void Xrotate(float theta);
    void Zrotate(float theta);

    void draw(GLuint shader);

private:
    // x축 회전을 보여주는 외부 틀
    std::vector<Cube*> XaxisFrame;

    // z축 회전을 보여주는 외부 틀
    std::vector<Cube*> ZaxisFrame;

    // 회전에 따라 돌아가는 손잡이 ( 나중에 육각형말고 원통으로 바꿔야 더 좋을 듯 )
    std::vector<Cube*> Xhandle;
    std::vector<Cube*> Zhandle;

    // 바닥
    Cube* base;

    // 일정 회전 이상은 금지용
    float rotatedAmountX = 0.0f;
    float maxRotationX = 10.0f;

    float rotatedAmountZ = 0.0f;
    float maxRotationZ = 10.0f;
};