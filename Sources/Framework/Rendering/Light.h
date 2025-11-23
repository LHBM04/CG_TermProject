#pragma once
#pragma once

#include "../PCH.h"
#include "Cube.h"

class Light
{
public:
    Light(Vector3 pos_, Vector3 color_ = Vector3(1.0f, 1.0f, 1.0f))
        : box(new Cube())
        , pos(pos_)
        , color(color_)
    {
        box->move(pos);
    }
    void turnOn()
    {
        color = Vector3(1.0f);
    }
    void turnOff()
    {
        color = Vector3(0.1f);
    }

    void move(Vector3 v)
    {
        pos += v;
        box->move(v);
    }

    void teleport(Vector3 v)
    {
        pos = v;
        box->teleport(v);
    }

    void applyLight(GLuint shaderProgram)
    {
        unsigned int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3f(lightPosLocation, pos.x, pos.y, pos.z);

        int lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3f(lightColorLocation, color.x, color.y, color.z);

        box->Draw(shaderProgram);
    }

private:
    // 조명이 어디 있는지 보기 위한 작은 큐브
    Cube*     box;

    Vector3   pos;
    Vector3   color;
};