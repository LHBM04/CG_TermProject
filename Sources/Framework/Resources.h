#pragma once

#include "Common.h"

class Resource
{
public:
    virtual ~Resource() noexcept = default;
};

template <typename TResource>
concept IsResource = std::derived_from<Resource, TResource>;

class Sprite : public Resource
{
};

class Texture : public Resource
{
};

class Font : public Resource
{
};

/**
 * @class Shader
 *
 * @brief 셰이더를 정의합니다.
 */
class Shader final
{
public:
    /**
     * @brief 셰이더를 사용합니다.
     */
    inline void Use() noexcept
    {
        glUseProgram(programID);
    }

    /**
     * @brief 셰이더 프로그램 ID를 반환합니다.
     *
     * @return unsigned int 셰이더 프로그램 ID.
     */
    [[nodiscard]]
    inline unsigned int GetProgramID() noexcept
    {
        return programID;
    }

    /**
     * @brief 정수형 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformInt(const char* const name_, const int value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniform1i(location, value_);
    }

    /**
     * @brief 실수형 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformFloat(const char* const name_, const float value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniform1f(location, value_);
    }

    /**
     * @brief 2차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformVector2(const char* const name_, const glm::fvec3& value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniform2fv(location, 1, glm::value_ptr(value_));
    }

    /**
     * @brief 3차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformVector3(const char* const name_, const glm::fvec3& value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniform3fv(location, 1, glm::value_ptr(value_));
    }

    /**
     * @brief 4차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformVector4(const char* const name_, const glm::vec4& value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniform4fv(location, 1, glm::value_ptr(value_));
    }

    /**
     * @brief 4x4 행렬 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformMatrix4x4(const char* const name_, const glm::mat4& value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value_));
    }

private:
    /**
     * @brief 셰이더 프로그램 ID.
     */
    unsigned int programID;
};

class Material : public Resource
{
};

class Mesh final : public Resource
{
public:
    /**
     * @struct Vertex
     *
     * @brief 정점 데이터를 정의합니다.
     */
    struct Vertex final
    {
        /**
         * @brief 정점의 위치.
         */
        glm::vec3 position;

        /**
         * @brief 정점의 법선 벡터.
         */
        glm::vec3 normal;

        /**
         * @brief 정점의 텍스처 좌표.
         */
        glm::vec3 texCoords;
    };

    /**
     * @brief 생성자.
     */
    Mesh() noexcept;

    /**
     * @brief 소멸자.
     */
    ~Mesh() noexcept;

private:
    /**
     * @brief 정점 배열 객체.
     */
    unsigned int vao;

    /**
     * @brief 정점 버퍼 객체.
     */
    unsigned int vbo;

    /**
     * @brief 요소 배열 객체.
     */
    unsigned int ebo;

    /**
     * @brief 
     */
    std::vector<Mesh::Vertex> vertices;

    /**
     * @brief 
     */
    std::vector<unsigned int> indices;
};

class AudioClip : public Resource
{
public:

private:
};

/**
 * @class ResourceManager
 * 
 * @brief 
 */
class ResourceManager final
{
    STATIC_CLASS(ResourceManager)

public:
    template <IsResource TResource>
    static TResource* LoadResource(std::string_view path_)
    {
        if (resources.contains(path_))
        {
            return static_cast<TResource*>(resources[path_].get());
        }

        auto result = MakeUnique<TResource>();
        

        return resources.emplace(path_, std::move(result)).first->second.get();
    }

    template <IsResource TResource>
    static TResource* GetResource(std::string_view path_)
    {
        auto it = resources.find(path_);
        if (it != resources.end())
        {
            return static_cast<TResource*>(it->second.get());
        }

        return nullptr;
    }

private:
    /**
     * @brief 게임 내 사용할 리소스들.
     */
    static std::unordered_map<std::string, std::unique_ptr<Resource>> resources;
};