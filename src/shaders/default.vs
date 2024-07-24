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
	f_position = vec3(model * vec4(v_position, 1.0));
	gl_Position = projection * view * vec4(f_position, 1.0);
	f_uv = v_uv;
	f_normal = normalize(mat3(transpose(inverse(model))) * v_normal);
}