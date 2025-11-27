#include "Renderer.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

Renderer::Renderer(const Window& window_) noexcept
{
	
}

Renderer::~Renderer() noexcept
{
}

void Renderer::Clear() const noexcept
{
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Present() const noexcept
{
    glfwSwapBuffers(windo)
}