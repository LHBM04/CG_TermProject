#include "Shader.h"

#include <stdexcept>

bool Shader::Initialize() noexcept
{
    GLuint vertexShader   = CompileShader(GL_VERTEX_SHADER, "Resources/Shaders/Vertex.vert");
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, "Resources/Shaders/Fragment.frag");

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(programID);
    isUsing = true;

    return true;
}

void Shader::Release() noexcept
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}

GLuint Shader::CompileShader(GLenum type_, std::string_view path_) noexcept
{
    // TODO: Resources로 셰이더 소스 불러오기
    // const std::string code = Resources::GetShader(path_);
    // const GLchar*     src  = code.c_str();

     // GLuint shader = glCreateShader(type_);
     // glShaderSource(shader, 1, &src, nullptr);
     // glCompileShader(shader);
     // 
     // return shader;
    throw std::runtime_error("No imple");
}

bool Shader::isUsing = false;

GLuint Shader::programID = 0;