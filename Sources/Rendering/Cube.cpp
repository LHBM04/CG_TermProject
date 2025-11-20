#include "Cube.h"
#include "../Core/Time.h"

Cube::Cube(glm::vec3 pos_, glm::vec3 color)
{
    vertices = 
    {
        {{+1.0f, +1.0f, +1.0f}, color, {0, 0, 1}},
        {{+1.0f, -1.0f, +1.0f}, color, {0, 0, 1}},
        {{-1.0f, -1.0f, +1.0f}, color, {0, 0, 1}},
        {{-1.0f, +1.0f, +1.0f}, color, {0, 0, 1}},
        {{+1.0f, +1.0f, -1.0f}, color, {0, 0, -1}},
        {{+1.0f, -1.0f, -1.0f}, color, {0, 0, -1}},
        {{-1.0f, -1.0f, -1.0f}, color, {0, 0, -1}},
        {{-1.0f, +1.0f, -1.0f}, color, {0, 0, -1}},
        {{+1.0f, +1.0f, -1.0f}, color, {1, 0, 0}},
        {{+1.0f, -1.0f, -1.0f}, color, {1, 0, 0}},
        {{+1.0f, -1.0f, +1.0f}, color, {1, 0, 0}},
        {{+1.0f, +1.0f, +1.0f}, color, {1, 0, 0}},
        {{-1.0f, +1.0f, +1.0f}, color, {-1, 0, 0}},
        {{-1.0f, -1.0f, +1.0f}, color, {-1, 0, 0}},
        {{-1.0f, -1.0f, -1.0f}, color, {-1, 0, 0}},
        {{-1.0f, +1.0f, -1.0f}, color, {-1, 0, 0}},
        {{+1.0f, +1.0f, -1.0f}, color, {0, 1, 0}},
        {{+1.0f, +1.0f, +1.0f}, color, {0, 1, 0}},
        {{-1.0f, +1.0f, +1.0f}, color, {0, 1, 0}},
        {{-1.0f, +1.0f, -1.0f}, color, {0, 1, 0}},
        {{+1.0f, -1.0f, +1.0f}, color, {0, -1, 0}},
        {{+1.0f, -1.0f, -1.0f}, color, {0, -1, 0}},
        {{-1.0f, -1.0f, -1.0f}, color, {0, -1, 0}},
        {{-1.0f, -1.0f, +1.0f}, color, {0, -1, 0}}
    };

    index = 
    {
        0,1,2,2,3,0,
        4,5,6,6,7,4,

        8,9,10,10,11,8,
        12,13,14,14,15,12,

        16,17,18,18,19,16,
        20,21,22,22,23,20
    };
    pos    = pos_;
    obb    = new OBB(pos_, glm::vec3(1.0f), glm::mat3(1.0f));
    initBuffer();
}

void Cube::initBuffer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // 위치 (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    // 색상 (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // 노멀벡터 (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    // index 연결하기
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), index.data(), GL_STATIC_DRAW);
}

void Cube::move(glm::vec3 v)
{
    pos += v;
    obb->move(v);
}

void Cube::checkCollisions(Cube*& target)
{
    // target과 obb충돌검사
    if (obb->testOBBOBB_SAT(*target->obb, mtv))
    {
        // 충돌 시 이동 방향을 나타내는 dir이 반사됨
        // 방향을 mtv로 바꿔야 되는데 mtv가 매우 작은 값이라 어떻게 할 지 생각중
        dir = -dir * cor;

        // 충돌했으면 일단 서로 안 충돌할 정도로만 밀어냄
        move(mtv);

        // 충돌 후 이동할 방향의 길이가 매우 작으면 붙은 것으로 판단하게 함
        if (getLength() <= 0.02f)
        {
            isStatic = true;
        }
    }
}

void Cube::teleport(glm::vec3 v)
{
    pos = v;
    obb->teleport(pos);
}

void Cube::changeColor(glm::vec3 color)
{
    for (auto& v : vertices)
    {
        v.color = color;
    }
}

void Cube::Update()
{
    if (isStatic) return;
    else
    {
        dir += gravity * Time::GetDeltaTime();

        pos = Vector3(pos.x + (dir.x * Time::GetDeltaTime()),
                      pos.y + (dir.y * Time::GetDeltaTime()),
                      pos.z + (dir.z * Time::GetDeltaTime()));
        obb->teleport(pos);
    }
}

void Cube::Draw(GLuint shaderProgram)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);

    glm::mat4 rot4(1.0f);
    rot4[0] = glm::vec4(glm::vec3(radius.x, 0.0f, 0.0f), 0.0f);
    rot4[1] = glm::vec4(glm::vec3(0.0f, radius.y, 0.0f), 0.0f);
    rot4[2] = glm::vec4(glm::vec3(0.0f, 0.0f, radius.z), 0.0f);

    model *= rot4;

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index.size()), GL_UNSIGNED_INT, 0);

    obb->draw(shaderProgram);
}
