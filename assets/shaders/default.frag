// Fragment shader
#version 410 core

uniform float u_time;
uniform sampler2D tex;

in vec2 fragTexCoord;

out vec4 color;

void main()
{
	color = texture(tex, fragTexCoord);
}