#pragma once

#include "../PCH.h"
#include "OBB.h"

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texture;
};

class Cube
{
public:
    Cube(glm::vec3 pos_ = glm::vec3(0.0f, 0.0f, 0.0f));
    
    void initBuffer();

    void setTexture(GLuint textureID)
    {
        texture = textureID;
    }

    const glm::vec3 getPos()
    {
        return pos;
    }
    const float getLength()
    {
        return sqrt(pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2));
    }

    void move(glm::vec3 v);
    void teleport(glm::vec3 v);
    void rotate(float theta, glm::vec3 axis);
    void rotate(float theta, glm::vec3 axis, glm::vec3 pivot);

    void resize(glm::vec3 radius_)
    {
        radius = radius_;
        obb->resize(radius_);
    }

    void Update();
    void checkCollisions(Cube* target);

    void Draw(GLuint shaderProgram);

private:
    GLuint VAO = 0, VBO = 0, EBO = 0;
    unsigned int              texture;

    std::vector<Vertex>       vertices;
    std::vector<unsigned int> index;

    OBB* obb;

    // 충돌 시에 둘을 분리시키는 최소 이동 벡터 저장용
    glm::vec3 mtv; 

    // 위치와 반지름
    // 예시1) radius.x 가 1.0f이고 y,z가 0이면 길이가 2.0f인 선분이 그려짐
    // 예시2) radius가 x,y,z 모두 1.0f면 반지름이 2인 육면체가 나옴
    Vector3 pos = Vector3(0.0f,0.0f,0.0f);
    Vector3 radius = Vector3(1.0f);

    // 회전
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // 이동하려는 방향. 속도 포함하고 있는데 분리해야됨
    Vector3 dir = Vector3(0.0f, 0.0f, 0.0f);

    // 중력
    const Vector3 gravity = Vector3(0.0f, -9.8f, 0.0f);

    // 큐브의 반발 계수
    const float cor = 0.3f;
};