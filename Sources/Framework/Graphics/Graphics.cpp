#include "Graphics.hpp"

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../Core/Debug/Logger.hpp"

bool Graphics::Initialize()
{
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        Logger::Critical("Failed to initialize GLAD");
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

#if defined(DEBUG) || defined(_DEBUG)
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OnDebugMessage, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
#endif

    return true;
}

void Graphics::OnDebugMessage(
    const GLenum  source_,
    const GLenum  type_,
    const GLuint  id_,
    const GLenum  severity_,
    const GLsizei length_,
    const GLchar* message_,
    const GLvoid* userParam_) noexcept
{
    switch (severity_)
    {
        case GL_DEBUG_SEVERITY_HIGH:
        {
            Logger::Error("[OpenGL][High]{}", message_);
            break;
        }
        case GL_DEBUG_SEVERITY_MEDIUM:
        {
            Logger::Warn("[OpenGL][Medium]{}", message_);
            break;
        }
        case GL_DEBUG_SEVERITY_LOW:
        {
            Logger::Warn("[OpenGL][Low] {}", message_);
            break;
        }
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        {
            Logger::Warn("[OpenGL][Notification] {}", message_);
            break;
        }
        default:
        {
            break;
        }
    }
}