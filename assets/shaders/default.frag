// Fragment shader
#version 410 core

uniform float u_time;
uniform sampler2D tex;

in vec3 fragPosition;
in vec2 fragUv;
in vec3 fragNormal;

out vec4 color;

uniform vec3 u_lightPos;

void main()
{
	// Ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

	// Diffuse
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(u_lightPos - fragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

	// color = texture(tex, fragUv) * vec4(ambient + diffuse, 1.0);
	// color = texture(tex, fragUv);
	// color = vec4(1.0);

	color = vec4(abs(fragNormal), 1.0);
}
