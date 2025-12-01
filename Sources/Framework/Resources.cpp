#include "Resources.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Debug.h"
#include "IO.h"

Resource::~Resource() noexcept
{
}

#pragma region Texture Implementation
Texture::Texture() noexcept
    : width(0)
    , height(0)
    , channels(0)
    , textureID(0)
{
}

Texture::~Texture() noexcept
{
    if (textureID)
    {
        glDeleteTextures(1, &textureID);
    }
}

bool Texture::Load(const std::filesystem::path& path_) noexcept
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path_.string().c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        Logger::Error("Failed to load texture image: {}", path_.string());
        return false;
    }

    GLenum format;
    if (channels == 1)
    {
        format = GL_RED;
    }
    else if (channels == 3)
    {
        format = GL_RGB;
    }
    else if (channels == 4)
    {
        format = GL_RGBA;
    }
    else
    {
        format = GL_RGB;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    Logger::Info("Texture loaded successfully: {} ({}x{}, {}ch)", path_.string(), width, height, channels);
    return true;
}
#pragma endregion

#pragma region Shader Implementation
Shader::Shader() noexcept
    : programID(0)
{
}

Shader::~Shader() noexcept
{
    if (programID)
    {
        glDeleteProgram(programID);
    }
}

bool Shader::Load(const std::filesystem::path& path_) noexcept
{
    std::filesystem::path vertexPath = path_;
    vertexPath += ".vert";

    std::filesystem::path fragmentPath = path_;
    fragmentPath += ".frag";

    const std::string vertexCode   = File::ReadAllText(vertexPath);
    const std::string fragmentCode = File::ReadAllText(fragmentPath);

    if (vertexCode.empty())
    {
        Logger::Error("Vertex shader source is empty or file not found: {}", vertexPath.string());
        return false;
    }
    if (fragmentCode.empty())
    {
        Logger::Error("Fragment shader source is empty or file not found: {}", fragmentPath.string());
        return false;
    }

    const unsigned int vertexShader = Shader::Compile(GL_VERTEX_SHADER, vertexCode);
    if (vertexShader == 0)
        return false;

    const unsigned int fragmentShader = Shader::Compile(GL_FRAGMENT_SHADER, fragmentCode);
    if (fragmentShader == 0)
        return false;

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        char infoLog[1024];
        glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
        Logger::Error("Shader Linking Failed [{}]:\n{}", path_.string(), infoLog);
        glDeleteProgram(programID);
        programID = 0;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Logger::Info("Shader loaded successfully: {}", path_.string());
    return true;
}

unsigned int Shader::Compile(unsigned int type_, std::string_view source_) noexcept
{
    const unsigned int shader     = glCreateShader(type_);
    const char*        sourceCStr = source_.data();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        Logger::Error("Shader Compilation Failed (Type: {}):\n{}", type_, infoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
#pragma endregion

#pragma region Mesh Implementation
Mesh::Mesh() noexcept
    : vao(0)
    , vbo(0)
    , ebo(0)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

Mesh::~Mesh() noexcept
{
    if (vao != 0)
    {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
    }
    if (ebo != 0)
    {
        glDeleteBuffers(1, &ebo);
    }
}

void Mesh::Draw() noexcept
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

bool Mesh::Load(const std::filesystem::path& path_) noexcept
{
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      warn, err;

    std::filesystem::path baseDirPath = Path::GetDirectoryName(path_);
    std::string           baseDir     = baseDirPath.string() + "/";

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path_.string().c_str(), baseDir.c_str());

    if (!warn.empty())
        Logger::Warn("Mesh Load Warning [{}]: {}", path_.string(), warn);
    if (!err.empty())
        Logger::Error("Mesh Load Error [{}]: {}", path_.string(), err);
    if (!ret)
        return false;

    vertices.clear();
    indices.clear();

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            if (index.vertex_index >= 0)
            {
                vertex.position = glm::fvec3(attrib.vertices[3 * index.vertex_index + 0],
                                             attrib.vertices[3 * index.vertex_index + 1],
                                             attrib.vertices[3 * index.vertex_index + 2]);
            }

            if (index.normal_index >= 0)
            {
                vertex.normal = glm::fvec3(attrib.normals[3 * index.normal_index + 0],
                                           attrib.normals[3 * index.normal_index + 1],
                                           attrib.normals[3 * index.normal_index + 2]);
            }

            if (index.texcoord_index >= 0)
            {
                vertex.texCoords = glm::fvec2(attrib.texcoords[2 * index.texcoord_index + 0],
                                              attrib.texcoords[2 * index.texcoord_index + 1]);
            }

            vertices.push_back(vertex);
            indices.push_back(static_cast<unsigned int>(indices.size()));
        }
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);

    Logger::Info("Mesh loaded successfully: {} (Vertices: {}, Indices: {})",
                 path_.string(),
                 vertices.size(),
                 indices.size());
    return true;
}
#pragma endregion

std::unordered_map<std::filesystem::path, std::unique_ptr<Resource>> ResourceManager::resources;