#include "Cube.h"
#include "../Core/Time.h"
#include <iostream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Cube::Cube(glm::vec3 pos_, const char* str)
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
    obb    = new OBB(pos_, glm::vec3(1.0f), glm::mat3(1.0f));

    initBuffer();
    initTexture();
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

    // 노말 (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // 텍스쳐 (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glEnableVertexAttribArray(2);

    // index 연결하기
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), index.data(), GL_STATIC_DRAW);
}

void Cube::initTexture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // 래핑
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 먼저 경로/존재 여부 로깅
    const char* relPath = "Sources/Rendering/wood_texture1.png";
    std::cout << "[Texture] cwd: " << std::filesystem::current_path().string() << std::endl;
    std::cout << "[Texture] try: " << relPath
              << " exists: " << (std::filesystem::exists(relPath) ? "true" : "false") << std::endl;

    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(relPath, &width, &height, &numberOfChannel, 0);
    if (!data)
    {
        std::cerr << "[Texture] load failed: " << relPath
                  << " reason: " << (stbi_failure_reason() ? stbi_failure_reason() : "unknown") << std::endl;

        // 폴백 1x1 텍스처(레벨0만) 생성 -> 베이스 레벨 정의
        const unsigned char white[4] = { 255, 255, 255, 255 };
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 밉맵 없음
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
        return;
    }

    // 성공 시: 밉맵 필터
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum externalFormat = GL_RGB;
    GLenum internalFormat = GL_RGB8;
    if (numberOfChannel == 1) { externalFormat = GL_RED;  internalFormat = GL_R8;    }
    else if (numberOfChannel == 3) { externalFormat = GL_RGB;  internalFormat = GL_RGB8;  }
    else if (numberOfChannel == 4) { externalFormat = GL_RGBA; internalFormat = GL_RGBA8; }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, externalFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    int levels = 1 + static_cast<int>(std::floor(std::log2(std::max(width, height))));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, levels - 1);

    stbi_image_free(data);
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
    rotation    = q * rotation;
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
    rotation    = q * rotation;
}

void Cube::checkCollisions(Cube* target)
{
    // target과 obb충돌검사
    if (obb->testOBBOBB_SAT(*target->obb, mtv))
    {
        // 충돌했으면 일단 서로 안 충돌할 정도로만 밀어냄
        move(mtv);

        glm::vec3 normal  = glm::normalize(mtv);
        float     dotProd = glm::dot(dir, normal);

        dir = dir - (1.0f + cor) * dotProd * normal;
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
    dir += gravity * dt;
    pos += dir * dt;
    obb->teleport(pos);
}

void Cube::Draw(GLuint shaderProgram)
{
    // 정적 정점 데이터 재업로드 제거
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
    model *= glm::mat4_cast(rotation);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), radius);
    model *= scale;

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint samplerLoc = glGetUniformLocation(shaderProgram, "outTexture");
    glUniform1i(samplerLoc, 0);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index.size()), GL_UNSIGNED_INT, 0);

    obb->draw(shaderProgram);
}
