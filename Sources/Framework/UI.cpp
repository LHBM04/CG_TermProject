#include "UI.h"

#include "Application.h"
#include "Resources.h"

ImageRenderer::ImageRenderer(Object* const owner) noexcept
    : Component(owner)
    , shader(nullptr)
    , mesh(nullptr)
    , texture(nullptr)
{
}

ImageRenderer::~ImageRenderer() noexcept
{
}

void ImageRenderer::Render() noexcept
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->Use();

    float width  = (float)Application::GetWindowWidth();
    float height = (float)Application::GetWindowHeight();

    glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
    shader->SetUniformMatrix4x4("projection", projection);

    shader->SetUniformMatrix4x4("model", GetTransform()->GetWorldMatrix());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    shader->SetUniformInt("outTexture", 0);

    shader->SetUniformVector4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0));

    mesh->Draw();
}

TextRenderer::TextRenderer(Object* const owner) noexcept
    : Component(owner)
    , shader(nullptr)
    , font(nullptr)
    , text("")
    , color(1.0f, 1.0f, 1.0f, 1.0f)
    , VAO(0)
    , VBO(0)
{
    // Texture를 사용하는 게 이상적인데 확장할 시간이 없다.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer() noexcept
{
    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (VBO)
        glDeleteBuffers(1, &VBO);
}

void TextRenderer::Render() noexcept
{
    if (!shader || !font)
        return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->Use();

    float     width      = (float)Application::GetWindowWidth();
    float     height     = (float)Application::GetWindowHeight();
    glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

    shader->SetUniformMatrix4x4("projection", projection);
    shader->SetUniformVector4("textColor", color);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);

    glm::vec3 pos = GetTransform()->GetPosition();
    float     x   = pos.x;
    float     y   = pos.y;

    float scale = GetTransform()->GetScale().x;
    const auto& characters = font->GetCharacters();

    for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
    {
        Font::Character ch = characters.at(*c);

        // [추가] 공백 문자 등 크기가 없는 글자는 렌더링 스킵 (경고 해결)
        if (ch.size.x == 0 || ch.size.y == 0)
        {
            x += (ch.advance >> 6) * scale;
            continue;
        }

        float xpos = x + ch.bearing.x * scale;
        float ypos = y + (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {{xpos, ypos - h, 0.0f, 0.0f},
                                {xpos, ypos, 0.0f, 1.0f},
                                {xpos + w, ypos, 1.0f, 1.0f},

                                {xpos, ypos - h, 0.0f, 0.0f},
                                {xpos + w, ypos, 1.0f, 1.0f},
                                {xpos + w, ypos - h, 1.0f, 0.0f}};

        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}