#pragma once

#include "Common.h"
#include "Objects.h"

class Shader;
class Mesh;
class Texture;
class Font;

class ImageRenderer : public Component
{
public:
    /**
     * @brief 생성자.
     *
     * @param owner 해당 컴포넌트의 오너
     */
    explicit ImageRenderer(Object* const owner) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~ImageRenderer() noexcept override;

    [[nodiscard]]
    inline Shader* GetShader() const noexcept
    {
        return shader;
    }
    inline void SetShader(Shader* shader_) noexcept
    {
        shader = shader_;
    }

    inline Mesh* GetMesh() const noexcept
    {
        return mesh;
    }
    inline void SetMesh(Mesh* mesh_) noexcept
    {
        mesh = mesh_;
    }

    [[nodiscard]]
    inline Texture* GetTexture() const noexcept
    {
        return texture;
    }
    inline void SetTexture(Texture* texture_) noexcept
    {
        texture = texture_;
    }

protected:
    virtual void Render() noexcept override;

private:
    Shader*  shader;
    Mesh*    mesh;
    Texture* texture;
};

class TextRenderer : public Component
{
public:
    /**
     * @brief 생성자.
     *
     * @param owner 해당 컴포넌트의 오너
     */
    explicit TextRenderer(Object* const owner) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~TextRenderer() noexcept override;

    [[nodiscard]]
    inline Shader* GetShader() const noexcept
    {
        return shader;
    }
    inline void SetShader(Shader* shader_) noexcept
    {
        shader = shader_;
    }

    [[nodiscard]]
    inline Mesh* GetMesh() const noexcept
    {
        return mesh;
    }
    inline void SetMesh(Mesh* mesh_) noexcept
    {
        mesh = mesh_;
    }

    [[nodiscard]]
    inline Font* GetFont() const noexcept
    {
        return font;
    }
    inline void SetFont(Font* font_) noexcept
    {
        font = font_;
    }

    [[nodiscard]]
    inline float GetFontSize() const noexcept
    {
        return fontSize;
    }
    inline void SetFontSize(float fontSize_) noexcept
    {
        fontSize = fontSize_;
    }

    [[nodiscard]]
    inline glm::vec4 GetColor() const noexcept
    {
        return color;
    }
    inline void SetColor(const glm::vec4& color_) noexcept
    {
        color = color_;
    }
    
    [[nodiscard]]
    inline std::string GetText() const noexcept
    {
        return text;
    }
    inline void SetText(std::string_view text_) noexcept
    {
        text = text_;
    }

protected:
    virtual void Render() noexcept override;

private:
    Shader* shader;
    Mesh*   mesh;
    Font*   font;
    float   fontSize;
    glm::vec4    color;
    std::string text;
    unsigned int VAO;
    unsigned int VBO;
};

class Button : public Component
{
private:
};