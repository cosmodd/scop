// Fragment shader
#version 410 core

uniform float u_time;
uniform sampler2D tex;

in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 color;

void main()
{
	float sine = sin(u_time * 4) * 0.5 + 0.5;
	// color = vec4(fragColor * sine, 1.0);
	color = texture(tex, fragTexCoord) * vec4(fragColor, 1.0);
}