#pragma once

#include "Common.h"

class Resource
{

};

class Sprite : public Resource
{
};

class Texture : public Resource
{
};

class Font : public Resource
{
};

class Shader : public Resource
{
};

class Material : public Resource
{
};

class Mesh : public Resource
{
};

class AudioClip : public Resource
{
};

class Resources final
{
    STATIC_CLASS(Resources)
};