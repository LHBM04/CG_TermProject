#include "Graphics.h"

Graphics::Graphics(const Graphics::Specification& specification_) noexcept
    : specification(specification_)
{
    int result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    if (result == 0)
    {
        // Handle initialization failure
    }

    glViewport(specification.x, specification.y, specification.width, specification.height);
}

Graphics::~Graphics()
{
}

void Graphics::PreRender()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::PostRender()
{
}