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

    glm::vec3 v = dir * speedScale * dt;
    pos += v;
    obb->teleport(pos);

    float speed = glm::length(v);
    if (speed > 1e-6f)
    {
        // 수평 이동 방향 (지면 탄젠트 성분)
        glm::vec3 moveDir = glm::normalize(v);
        glm::vec3 axis    = glm::cross(n, moveDir);

        float axisLen = glm::length(axis);
        if (axisLen > 1e-6f)
        {
            axis = axis / axisLen;

            // 유효 반지름 추정: 메시 스케일 평균 사용 (보다 정밀히는 접촉 방향 반지름)
            // radius가 반지름 스케일 벡터이므로 평균값을 사용
            float effectiveRadius = (radius.x + radius.y + radius.z) / 3.0f;
            // 보호: 너무 작은 반지름 방지
            effectiveRadius = glm::max(effectiveRadius, 0.4f);

            float angleRad = speed / effectiveRadius; // s = r * theta
            float angleDeg = glm::degrees(angleRad);

            glm::quat q = glm::angleAxis(angleRad, axis);
            rotation    = glm::normalize(q * rotation);

            obb->rotate(angleDeg, axis);
        }
    }

    // 디버그
    SPDLOG_INFO("dir = ( {}, {}, {} ), pos = ( {}, {}, {} )", dir.x, dir.y, dir.z, pos.x, pos.y, pos.z);
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

void Cube::LoadFromModel(const Model& model)
{
    if (model.vertex_count == 0 || model.face_count == 0)
    {
        SPDLOG_WARN("Cube::LoadFromModel: empty model.");
        return;
    }

    // 기존 GL 리소스 정리
    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (VBO)
        glDeleteBuffers(1, &VBO);
    if (EBO)
        glDeleteBuffers(1, &EBO);
    VAO = VBO = EBO = 0;

    vertices.clear();
    index.clear();
    vertices.reserve(model.face_count * 3);
    index.reserve(model.face_count * 3);

    glm::vec3 minP(FLT_MAX);
    glm::vec3 maxP(-FLT_MAX);

    // Face 단위로 삼각형 생성 (중복 정점)
    for (size_t i = 0; i < model.face_count; ++i)
    {
        const Face&  f     = model.faces[i];
        unsigned int vs[3] = {f.v1, f.v2, f.v3};
        unsigned int ts[3] = {f.t1, f.t2, f.t3};

        if (vs[0] >= model.vertex_count || vs[1] >= model.vertex_count || vs[2] >= model.vertex_count)
        {
            SPDLOG_WARN("Cube::LoadFromModel: vertex index out of range.");
            continue;
        }

        // 위치 가져오기
        glm::vec3 p[3];
        for (int k = 0; k < 3; ++k)
        {
            ::Vt mv = model.vertices[vs[k]];
            p[k]               = glm::vec3(mv.x, mv.y, mv.z);
            minP               = glm::min(minP, p[k]);
            maxP               = glm::max(maxP, p[k]);
        }

        // 노멀 계산
        glm::vec3 n = glm::normalize(glm::cross(p[1] - p[0], p[2] - p[0]));
        if (!glm::all(glm::isinf(n)))
            n = glm::vec3(0.0f, 1.0f, 0.0f);

        // 텍스처 좌표
        glm::vec2 uv[3];
        for (int k = 0; k < 3; ++k)
        {
            if (model.texcoords && ts[k] < model.texcoord_count)
            {
                const TexCoord& tc = model.texcoords[ts[k]];
                uv[k]              = glm::vec2(tc.u, tc.v);
            }
            else
            {
                uv[k] = glm::vec2(0.0f, 0.0f);
            }
        }

        // 정점 push
        for (int k = 0; k < 3; ++k)
        {
            Vertex cv;
            cv.pos     = p[k];
            cv.normal  = n;
            cv.texture = uv[k];
            vertices.push_back(cv);
            index.push_back(static_cast<unsigned int>(vertices.size() - 1));
        }
    }

    // 중심 이동 및 반경 계산
    glm::vec3 center      = (minP + maxP) * 0.5f;
    glm::vec3 halfExtents = (maxP - minP) * 0.5f;
    if (halfExtents.x <= 0.0f)
        halfExtents.x = 0.0001f;
    if (halfExtents.y <= 0.0f)
        halfExtents.y = 0.0001f;
    if (halfExtents.z <= 0.0f)
        halfExtents.z = 0.0001f;

    for (auto& v : vertices)
    {
        v.pos -= center; // 모델을 원점 중심 정렬
    }

    radius = Vector3(halfExtents.x * 0.5f, halfExtents.y * 0.5f, halfExtents.z * 0.5f);
    obb->resize(glm::vec3(radius.x * 0.5f, radius.y * 0.5f, radius.z * 0.5f));
    obb->teleport(glm::vec3(pos.x, pos.y, pos.z));

    initBuffer();

    SPDLOG_INFO("Cube::LoadFromModel: loaded {} faces ({} vertices generated).", model.face_count, vertices.size());
}
