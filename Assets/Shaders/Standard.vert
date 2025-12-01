#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 FragPos;
out vec2 TexCoord;

void main()
{
	gl_Position =  projection * view * model * vec4(in_position, 1.0);
    // 문제점: 모델 행렬(model)을 곱하지 않아 로컬 좌표가 그대로 들어감
	FragPos = in_position; 
    // 문제점: 회전 시 법선 벡터(Normal)가 같이 회전하지 않음
	normal = in_normal;
	TexCoord = in_texture;
}