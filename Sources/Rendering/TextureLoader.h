#pragma once

#include "../PCH.h"

class TextureLoader
{
public:
    static GLuint LoadTexture(const char* filename, GLsizei& width, GLsizei& height, GLsizei& channels);
};