#pragma once

typedef struct
{
    float x, y, z;
} Vt;

typedef struct
{
    float u, v;
} TexCoord;

typedef struct
{
    unsigned int v1, v2, v3;
    unsigned int t1, t2, t3;
} Face;

typedef struct
{
    Vt*   vertices;
    size_t    vertex_count;
    TexCoord* texcoords;
    size_t    texcoord_count;
    Face*     faces;
    size_t    face_count;
} Model;