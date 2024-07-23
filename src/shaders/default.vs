#version 420

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_uv;
layout (location = 2) in vec3 v_normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 f_position;
out vec2 f_uv;
out vec3 f_normal;

void main()
{
	gl_Position = projection * view * model * vec4(v_position, 1.0);
	f_position = v_position;
	f_uv = v_uv;
	f_normal = mat3(transpose(inverse(model))) * v_normal;
}