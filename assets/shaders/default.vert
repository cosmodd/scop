// Vertex shader
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out vec3 fragPosition;
out vec2 fragUv;
out vec3 fragNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
	fragPosition = position;
	fragUv = uv;
	fragNormal = mat3(transpose(inverse(u_model))) * normal;
}