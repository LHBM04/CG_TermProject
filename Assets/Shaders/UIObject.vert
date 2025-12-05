#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal; 
layout (location = 2) in vec2 in_texture;

uniform mat4 model;
uniform mat4 projection; // Orthographic Projection (직교 투영)

out vec2 TexCoord;

void main()
{
    // View 행렬 제거: 카메라는 무시하고 화면 좌표계에 직접 매핑
    gl_Position = projection * model * vec4(in_position, 1.0);
    TexCoord = in_texture;
}