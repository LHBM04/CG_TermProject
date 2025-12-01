#pragma once

#include "Common.h"

class ResourceManager;

class Resource
{
    friend class ResourceManager;

public:
    /**
     * @brief 소멸자.
     */
    virtual ~Resource() noexcept;

    /**
     * @brief 리소스 이름을 반환합니다.
     * 
     * @return std::string 리소스 이름
     */
    [[nodiscard]]
    inline std::string GetName() const noexcept
    {
        return name;
    }

    /**
     * @brief 리소스 이름을 설정합니다.
     * 
     * @param name_ 리소스 이름
     */
    inline void SetName(std::string_view name_) noexcept
    {
        name = name_;
    }

    /**
     * @brief 리소스 경로를 반환합니다.
     * 
     * @return std::string 리소스 경로
     */
    [[nodiscard]]
    inline const std::filesystem::path& GetPath() const noexcept
    {
        return path;
    }

    /**
     * @brief 리소스 경로를 설정합니다.
     * 
     * @param path_ 리소스 경로
     */
    inline void SetPath(const std::filesystem::path& path_) noexcept
    {
        path = path_;
    }

    /**
     * @brief 리소스가 로드되었는지 여부를 반환합니다.
     * 
     * @return bool 리소스 로드 여부
     */
    [[nodiscard]]
    inline bool IsLoaded() const noexcept
    {
        return isLoaded;
    }

protected:
    /**
     * @brief 지정한 경로에 위치한 리소스를 불러옵니다.
     * 
     * @param path_ 불러올 리소스의 경로.
     * 
     * @return bool 리소스가 성공적으로 불러와졌는지 여부.
     */
    virtual bool Load(const std::filesystem::path& path_) noexcept = 0;

private:
    /**
     * @brief 리소스 이름.
     */
    std::string name;

    /**
     * @brief 리소스 경로.
     */
    std::filesystem::path path;

    /**
     * @brief 리소스가 로드되었는지 여부.
     */
    bool isLoaded = false;
};

template <typename TResource>
concept IsResource = std::derived_from<TResource, Resource>;

class Sprite : public Resource
{
public:
    /**
     * @brief 생성자.
     */
    virtual ~Sprite() noexcept override;

protected:
    /**
     * @brief 머티리얼을 로드합니다.
     *
     * @param path_ 머티리얼 이름
     *
     * @return bool 머티리얼 로드 성공 여부
     */
    virtual bool Load(const std::filesystem::path& path_) noexcept override;
};

class Texture : public Resource
{
public:
    /**
     * @brief 생성자.
     */
    explicit Texture() noexcept;

    /**
     * @brief 생성자.
     */
    virtual ~Texture() noexcept override;

    /**
     * @brief 해당 텍스쳐의 ID를 반환합니다.
     * 
     * @return unsigned int 해당 텍스쳐의 ID.
     */
    [[nodiscard]]
    inline unsigned int GetTextureID() const noexcept
    {
        return textureID;
    }

    /**
     * @brief 해당 텍스쳐의 너비를 반환합니다.
     * 
     * @return int 해당 텍스쳐의 너비.
     */
    [[nodiscard]]
    inline int GetWidth() const noexcept
    {
        return width;
    }

    /**
     * @brief 해당 텍스쳐의 높이를 반환합니다.
     * 
     * @return int 해당 텍스쳐의 높이.
     */
    [[nodiscard]]
    inline int GetHeight() const noexcept
    {
        return height;
    }

protected:
    /**
     * @brief 머티리얼을 로드합니다.
     *
     * @param path_ 머티리얼 이름
     *
     * @return bool 머티리얼 로드 성공 여부
     */
    virtual bool Load(const std::filesystem::path& path_) noexcept override;

private:
    /**
     * @brief 해당 텍스쳐의 ID.
     */
    unsigned int textureID;

    /**
     * @brief 해당 텍스쳐의 너비.
     */
    int width;

    /**
     * @brief 해당 텍스쳐의 높이.
     */
    int height;

    /**
     * @brief 해당 텍스쳐의 채널 수.
     */
    int channels;
};

class Font : public Resource
{
public:
    /**
     * @brief 생성자.
     */
    virtual ~Font() noexcept override;

protected:
    /**
     * @brief 머티리얼을 로드합니다.
     *
     * @param path_ 머티리얼 이름
     *
     * @return bool 머티리얼 로드 성공 여부
     */
    virtual bool Load(const std::filesystem::path& path_) noexcept override;
};

/**
 * @class Shader
 *
 * @brief 셰이더를 정의합니다.
 */
class Shader : public Resource
{
public:
    /**
     * @brief 생성자.
     */
    explicit Shader() noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Shader() noexcept override;

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
     * @return unsigned int 셰이더 프로그램 ID
     */
    [[nodiscard]]
    inline unsigned int GetProgramID() noexcept
    {
        return programID;
    }

    /**
     * @brief 정수형 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름
     * @param value_ 설정할 값
     */
    inline void SetUniformInt(const char* const name_, const int value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniform1i(location, value_);
    }

    /**
     * @brief 실수형 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름
     * @param value_ 설정할 값
     */
    inline void SetUniformFloat(const char* const name_, const float value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniform1f(location, value_);
    }

    /**
     * @brief 2차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름
     * @param value_ 설정할 값
     */
    inline void SetUniformVector2(const char* const name_, const glm::fvec3& value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniform2fv(location, 1, glm::value_ptr(value_));
    }

    /**
     * @brief 3차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름
     * @param value_ 설정할 값
     */
    inline void SetUniformVector3(const char* const name_, const glm::fvec3& value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniform3fv(location, 1, glm::value_ptr(value_));
    }

    /**
     * @brief 4차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름
     * @param value_ 설정할 값
     */
    inline void SetUniformVector4(const char* const name_, const glm::vec4& value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniform4fv(location, 1, glm::value_ptr(value_));
    }

    /**
     * @brief 4x4 행렬 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름
     * @param value_ 설정할 값
     */
    inline void SetUniformMatrix4x4(const char* const name_, const glm::mat4& value_) noexcept
    {
        const GLint location = glGetUniformLocation(programID, name_);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value_));
    }

protected:
    /**
     * @brief 셰이더를 로드합니다.
     * 
     * @param path_ 셰이더 이름
     * 
     * @return bool 셰이더 로드 성공 여부
     */
    virtual bool Load(const std::filesystem::path& path_) noexcept override;

private:
    /**
     * @brief 셰이더를 컴파일합니다.
     * 
     * @param type_   셰이더 타입 (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER 등)
     * @param source_ 셰이더 소스 코드
     */
    [[nodiscard]]
    static unsigned int Compile(unsigned int type_, std::string_view source_) noexcept;

    /**
     * @brief 셰이더 프로그램 ID.
     */
    unsigned int programID;
};

class Material : public Resource
{
public:
    /**
     * @brief 생성자.
     */
    explicit Material() noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Material() noexcept override;

    inline void SetShader(Shader* shader_) noexcept
    {
        shader = shader_;
    }

    [[nodiscard]] Shader* GetShader() const noexcept
    {
        return shader;
    }

    void SetTexture(Texture* texture_) noexcept
    {
        texture = texture_;
    }

    [[nodiscard]] Texture* GetTexture() const noexcept
    {
        return texture;
    }

    void SetColor(const glm::vec4& color_) noexcept
    {
        color = color_;
    }

    [[nodiscard]] const glm::vec4& GetColor() const noexcept
    {
        return color;
    }

    void Bind() const noexcept;

protected:
    virtual bool Load(const std::filesystem::path& path_) noexcept override;

private:
    Shader*   shader  = nullptr;
    Texture*  texture = nullptr;
    glm::vec4 color   = glm::vec4(1.0f); // 기본값: 흰색
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
        glm::vec2 texCoords;
    };

    /**
     * @brief 생성자.
     */
    Mesh() noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Mesh() noexcept override;

    /**
     * @brief 메쉬를 바인딩합니다.
     */
    void Draw() noexcept;

protected:
    /**
     * @brief 머티리얼을 로드합니다.
     *
     * @param path_ 머티리얼 이름
     *
     * @return bool 머티리얼 로드 성공 여부
     */
    virtual bool Load(const std::filesystem::path& path_) noexcept override;

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
    /**
     * @brief 생성자.
     */
    virtual ~AudioClip() noexcept override;

protected:
    /**
     * @brief 머티리얼을 로드합니다.
     *
     * @param path_ 머티리얼 이름
     *
     * @return bool 머티리얼 로드 성공 여부
     */
    virtual bool Load(const std::filesystem::path& path_) noexcept override;
};

/**
 * @class ResourceManager
 * 
 * @brief 
 */
class ResourceManager final
{
    friend class Resource;

    STATIC_CLASS(ResourceManager)

public:
    template <IsResource TResource>
    static TResource* LoadResource(const std::filesystem::path& path_)
    {
        if (resources.contains(path_))
        {
            return dynamic_cast<TResource*>(resources[path_].get());
        }

        std::unique_ptr<TResource> result = std::make_unique<TResource>();

        Resource* resourceBase = dynamic_cast<Resource*>(result.get());

        std::filesystem::path fullPath = std::filesystem::current_path() / path_;
        if (!resourceBase->Load(fullPath))
        {
            return nullptr;
        }

        resourceBase->SetPath(path_);

        auto it = resources.emplace(path_, std::move(result));

        return dynamic_cast<TResource*>(it.first->second.get());
    }

    template <IsResource TResource>
    static TResource* GetResource(const std::filesystem::path& path_)
    {
        auto it = resources.find(path_);
        if (it != resources.end())
        {
            return dynamic_cast<TResource*>(it->second.get());
        }

        return nullptr;
    }

private:
    /**
     * @brief 게임 내 사용할 리소스들.
     */
    static std::unordered_map<std::filesystem::path, std::unique_ptr<Resource>> resources;
};