#include "Rendering.h"

#include "Application.h"
#include "Debug.h"
#include "Resources.h"

Camera::Camera(Object* const owner_) noexcept
    : Component(owner_)
    , shader(nullptr)
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
    if (!shader)
    {
        Logger::Error("Camera: Failed to load default shader.");
        return;
    }   

    glViewport(viewport.x, viewport.y, viewport.width, viewport.height);

    shader->Use();

    const glm::mat4 view = GetViewMatrix();
    shader->SetUniformMatrix4x4("view", view);

    const glm::mat4 projection = GetProjectionMatrix();
    shader->SetUniformMatrix4x4("projection", projection);

    const glm::fvec3 viewPosition = GetTransform()->GetPosition();
    shader->SetUniformVector3("viewPos", viewPosition);
}

glm::fmat4x4 Camera::GetViewMatrix() const noexcept
{
    Transform* const transform = GetOwner()->GetTransform();
    if (!transform)
    {
        return glm::mat4(1.0f);
    }

    const glm::vec3 pos   = transform->GetPosition();
    const glm::vec3 front = transform->GetForward();
    const glm::vec3 up    = transform->GetUp();

    return glm::lookAt(pos, pos + front, up);
}

glm::fmat4x4 Camera::GetProjectionMatrix() const noexcept
{
    const float aspectRatio = viewport.width / viewport.height;

    switch (projection)
    {
        case Camera::Projection::Orthographic:
        {
            const float halfHeight = orthoSize * 0.5f;
            const float halfWidth  = halfHeight * aspectRatio;

            return glm::ortho(
                    -halfWidth, halfWidth, -halfHeight, halfHeight, clipingPlanes.nearPlane, clipingPlanes.farPlane);
        }
        case Camera::Projection::Perspective:
        {
            return glm::perspective(
                    glm::radians(fieldOfView), aspectRatio, clipingPlanes.nearPlane, clipingPlanes.farPlane);
        }
        default:
        {
            return glm::fmat4x4(1.0f);
        }
    }
}

Light::Light(Object* const owner_) noexcept
    : Component(owner_)
    , shader(nullptr)
    , color(1.0f, 1.0f, 1.0f)
    , intensity(0.5f)
{
}

Light::~Light() noexcept
{
}

void Light::Update() noexcept
{
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
    , shader(nullptr)
    , mesh(nullptr)
    , texture(nullptr)
{
}

MeshRenderer::~MeshRenderer() noexcept
{
}

void MeshRenderer::Render() noexcept
{
    if (!shader)
    {
        Logger::Error("MeshRenderer: Failed to load default shader.");
        return;
    }

    if (!mesh)
    {
        Logger::Warn("MeshRenderer: No mesh assigned to render.");
        return;
    }

    shader->Use();
    shader->SetUniformMatrix4x4("model", GetTransform()->GetWorldMatrix());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

    if (texture)
    {
        texture->Bind();
    }

    shader->SetUniformInt("outTexture", 0);

    mesh->Draw();
}