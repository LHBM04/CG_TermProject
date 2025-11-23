#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec3 in_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 objectColor;
out vec3 normal;
out vec3 FragPos;

void main()
{
	gl_Position =  projection * view * model * vec4(in_position, 1.0);

	FragPos = in_position;

	objectColor = in_color;

	normal = in_normal;
}