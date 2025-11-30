#include "Resources.h"

#include "Debug.h"

Mesh::Mesh() noexcept
    : vao(0)
    , vbo(0)
    , ebo(0)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO (Vertex Data)
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Mesh::Vertex), vertices.data(), GL_STATIC_DRAW);

    // EBO (Indices)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position (Location 0)
    glGenBuffers(1, &ebo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), 
        reinterpret_cast<void*>(offsetof(Mesh::Vertex, position)));

    // Normal (Location 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), 
        reinterpret_cast<void*>(offsetof(Mesh::Vertex, normal)));

    // TexCoords (Location 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), 
        reinterpret_cast<void*>(offsetof(Mesh::Vertex, texCoords)));

    glBindVertexArray(0); // Unbind
}

Mesh::~Mesh() noexcept
{
    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (vbo)
        glDeleteBuffers(1, &vbo);
    if (ebo)
        glDeleteBuffers(1, &ebo);
}