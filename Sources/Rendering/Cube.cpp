#include "Cube.h"
#include "../Core/Time.h"

Cube::Cube(glm::vec3 pos_)
{
    vertices = 
    {
        {{+1.0f, +1.0f, +1.0f}, {0, 0, 1 }, {0.333333f, 0.5f } },
        {{+1.0f, -1.0f, +1.0f}, {0, 0, 1 }, {0.333333f, 0.0f } },
        {{-1.0f, -1.0f, +1.0f}, {0, 0, 1 }, {0.0f     , 0.0f } },
        {{-1.0f, +1.0f, +1.0f}, {0, 0, 1 }, {0.0f     , 0.5f } },
        {{+1.0f, +1.0f, -1.0f}, {0, 0, -1}, {0.333333f, 0.5f } },
        {{+1.0f, -1.0f, -1.0f}, {0, 0, -1}, {0.333333f, 0.0f } },
        {{-1.0f, -1.0f, -1.0f}, {0, 0, -1}, {0.666666f, 0.0f } },
        {{-1.0f, +1.0f, -1.0f}, {0, 0, -1}, {0.666666f, 0.5f } },
        {{+1.0f, +1.0f, -1.0f}, {1, 0, 0 }, {1.0f     , 0.5f } },
        {{+1.0f, -1.0f, -1.0f}, {1, 0, 0 }, {1.0f     , 0.0f } },
        {{+1.0f, -1.0f, +1.0f}, {1, 0, 0 }, {0.0f     , 0.5f } },
        {{+1.0f, +1.0f, +1.0f}, {1, 0, 0 }, {0.0f     , 1.0f } },
        {{-1.0f, +1.0f, +1.0f}, {-1, 0, 0}, {0.666666f, 0.5f } },
        {{-1.0f, -1.0f, +1.0f}, {-1, 0, 0}, {0.666666f, 0.0f } },
        {{-1.0f, -1.0f, -1.0f}, {-1, 0, 0}, {0.333333f, 0.5f } },
        {{-1.0f, +1.0f, -1.0f}, {-1, 0, 0}, {0.333333f, 1.0f } },
        {{+1.0f, +1.0f, -1.0f}, {0, 1, 0 }, {0.333333f, 0.5f } },
        {{+1.0f, +1.0f, +1.0f}, {0, 1, 0 }, {0.666666f, 0.5f } },
        {{-1.0f, +1.0f, +1.0f}, {0, 1, 0 }, {1.0f     , 0.5f } },
        {{-1.0f, +1.0f, -1.0f}, {0, 1, 0 }, {0.666666f, 0.5f } },
        {{+1.0f, -1.0f, +1.0f}, {0, -1, 0}, {0.333333f, 1.0f } },
        {{+1.0f, -1.0f, -1.0f}, {0, -1, 0}, {0.666666f, 1.0f } },
        {{-1.0f, -1.0f, -1.0f}, {0, -1, 0}, {1.0f     , 1.0f } },
        {{-1.0f, -1.0f, +1.0f}, {0, -1, 0}, {0.666666f, 1.0f } }
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
    obb    = new OBB(pos_, glm::vec3(1.0f));

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), index.data(), GL_STATIC_DRAW);
}

void Cube::move(glm::vec3 v)
{
    pos += v;
    obb->move(v);
}

void Cube::rotate(float theta, glm::vec3 axis)
{
    obb->rotate(theta, axis);
    glm::quat q = glm::angleAxis(glm::radians(theta), glm::normalize(axis));
    rotation    = glm::normalize(q * rotation);
}

void Cube::rotate(float theta, glm::vec3 axis, glm::vec3 pivot)
{
    if (glm::dot(axis, axis) < 1e-8f)
        return;

    glm::vec3 center   = glm::vec3(pos.x, pos.y, pos.z);
    glm::vec3 toCenter = center - pivot;

    glm::mat4 R         = glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::normalize(axis));
    glm::vec3 rotated   = glm::vec3(R * glm::vec4(toCenter, 0.0f));
    glm::vec3 newCenter = pivot + rotated;

    pos = Vector3(newCenter.x, newCenter.y, newCenter.z);
    obb->teleport(newCenter);

    obb->rotate(theta, axis);
    glm::quat q = glm::angleAxis(glm::radians(theta), glm::normalize(axis));
    rotation    = glm::normalize(q * rotation);
}

void Cube::rotateLocal(float theta, glm::vec3 axis)
{
    glm::vec3 local;
    if (axis == glm::vec3(1.0f, 0.0f, 0.0f))
    {
        local = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else if (axis == glm::vec3(0.0f, 1.0f, 0.0f))
    {
        local = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else if (axis == glm::vec3(0.0f, 0.0f, 1.0f))
    {
        local = rotation * glm::vec3(0.0f, 0.0f, 1.0f);
    }

    rotation = glm::normalize(glm::angleAxis(glm::radians(theta), local) * rotation);
}

// 절대 회전을 설정하면서 OBB도 델타만큼 회전시켜 동기화
void Cube::setRotationAbsolute(const glm::quat& target)
{
    glm::quat t = glm::normalize(target);
    glm::quat c = glm::normalize(rotation);
    glm::quat delta = glm::normalize(t * glm::inverse(c));

    // w가 약간 음수일 때도 동일 회전을 나타내므로 부호 표준화
    if (delta.w < 0.0f)
    {
        delta = glm::quat(-delta.w, -delta.x, -delta.y, -delta.z);
    }

    // angle 계산 안전화
    float w = glm::clamp(delta.w, -1.0f, 1.0f);
    float angleRad = 2.0f * acosf(w);
    float s = sqrtf(std::max(0.0f, 1.0f - w * w));

    glm::vec3 axis(1.0f, 0.0f, 0.0f);
    if (s > 1e-6f)
        axis = glm::vec3(delta.x / s, delta.y / s, delta.z / s);

    float angleDeg = glm::degrees(angleRad);

    // 아주 작은 델타는 무시
    if (angleDeg > 1e-4f && glm::dot(axis, axis) > 1e-8f)
    {
        axis = glm::normalize(axis);
        obb->rotate(angleDeg, axis);
    }

    rotation = t;
}

void Cube::checkCollisions(Cube* target)
{
    if (obb->testOBBOBB_SAT(*target->obb, mtv))
    {
        // MTV 클램프: 비정상적으로 큰 보정 방지
        float len = glm::length(mtv);
        if (len > 2.0f) // 씬 스케일에 맞게 임계값 조정
        {
            mtv *= (2.0f / len);
        }

        move(mtv);

        glm::vec3 normal = glm::normalize(mtv);
        float dotProd = glm::dot(dir, normal);

        // 벽을 뚫고 다시 들어가지 않게
        if (dotProd < 0.0f)
        {
            dir = dir - (1.0f + cor) * dotProd * normal;
        }

        // 벽과 접촉 시 약간 밀어내는 용
        const float separationBias = 1e-3f;
        move(separationBias * normal);
    }
}

void Cube::teleport(glm::vec3 v)
{
    pos = v;
    obb->teleport(pos);
}

void Cube::Update()
{
    float dt = Time::GetDeltaTime();
    // 중력 강화
    dir += gravity * gravityScale * dt;
    // 최종 속도 승수
    pos += dir * speedScale * dt;
    obb->teleport(pos);

    // 최대 속도 제한용
    float maxSpeed = 30.0f;
    float cur      = glm::length(glm::vec3(dir.x, dir.y, dir.z));
    if (cur > maxSpeed)
        dir *= (maxSpeed / cur);
}

// 경사면 노말을 외부에서 전달받아 추가 가속 적용
void Cube::UpdateWithSlope(const glm::vec3& groundNormal)
{
    float dt = Time::GetDeltaTime();
    dir += gravity * gravityScale * dt;

    // 경사면 추가 가속
    glm::vec3 n = glm::normalize(groundNormal);
    glm::vec3 g = glm::vec3(gravity.x, gravity.y, gravity.z) * gravityScale;
    glm::vec3 tangent = g - (glm::dot(g, n)) * n;
    if (slopeBoost > 0.0f)
    {
        dir += tangent * slopeBoost * dt;
    }

    pos += dir * speedScale * dt;
    obb->teleport(pos);
}

void Cube::Draw(GLuint shaderProgram)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
    model *= glm::mat4_cast(rotation);
    glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), radius);
    model *= scaleM;

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint samplerLoc = glGetUniformLocation(shaderProgram, "outTexture");
    glUniform1i(samplerLoc, 0);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index.size()), GL_UNSIGNED_INT, 0);

    obb->draw(shaderProgram);
}
