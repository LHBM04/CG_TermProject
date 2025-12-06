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