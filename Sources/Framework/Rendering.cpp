#include "Rendering.h"

#include "Application.h"
#include "Debug.h"
#include "Resources.h"

Camera::Camera(Object* const owner_) noexcept
    : Component(owner_)
    , projection(Projection::Perspective)
    , fieldOfView(60.0f)
    , clipingPlanes(0.1f, 100.0f)
    , orthoSize(10.0f)
{
    const float windowWidth  = Application::GetWindowWidth();
    const float windowHeight = Application::GetWindowHeight();
    viewport                 = {0.0f, 0.0f, windowWidth, windowHeight};

    transform->SetPosition(glm::fvec3(0.0f, 0.0f, 5.0f));
    transform->SetRotation(glm::fvec3(0.0f, 90.0f, 0.0f));
}

Camera::~Camera()
{
}

void Camera::Ready() const noexcept
{
    static Shader* shader = ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard");
    if (!shader)
    {
        Logger::Error("Camera: Failed to load default shader.");
        return;
    }

    shader->Use();

    glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
    
    const glm::mat4 view = GetViewMatrix();
    shader->SetUniformMatrix4x4("view", view);
    
    const glm::mat4 projection = GetProjectionMatrix();
    shader->SetUniformMatrix4x4("projection", projection);

    const glm::fvec3 viewPosition = GetTransform()->GetPosition();
    shader->SetUniformVector3("viewPos", viewPosition);
}

Light::Light(Object* const owner_) noexcept
    : Component(owner_)
    , color(1.0f, 1.0f, 1.0f)
    , intensity(0.5f)
{
}

Light::~Light() noexcept
{
}

void Light::Update() noexcept
{
    static Shader* shader = ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard");
    if (!shader)
    {
        Logger::Error("Light: Failed to load default shader.");
        return;
    }

    shader->Use();
    shader->SetUniformVector3("lightDir", GetTransform()->GetForward());
    shader->SetUniformVector3("lightColor", color * intensity);
    // shader->SetUniformVector3("lightPos", GetTransform()->GetPosition());
}

MeshRenderer::MeshRenderer(Object* const owner_) noexcept
    : Component(owner_)
{
}

MeshRenderer::~MeshRenderer() noexcept
{
}

void MeshRenderer::Render() noexcept
{
    if (!mesh)
    {
        Logger::Warn("MeshRenderer: No mesh assigned to render.");
        return;
    }

    static Shader* const shader = ResourceManager::LoadResource<Shader>("Assets\\Shaders\\Standard");
    if (!shader)
    {
        Logger::Error("MeshRenderer: Failed to load default shader.");
        return;
    }

    shader->Use();
    shader->SetUniformMatrix4x4("model", GetTransform()->GetWorldMatrix());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

    GLint samplerLoc = glGetUniformLocation(shader->GetProgramID(), "outTexture");
    glUniform1i(samplerLoc, 0);

    mesh->Draw();
}