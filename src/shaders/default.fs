#version 420

out vec4 FragColor;

in vec3 f_position;
in vec2 f_uv;
in vec3 f_normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D objectTexture;
uniform bool showNormal;

void main()
{
	if (!showNormal && texture(objectTexture, f_uv).a < 0.1)
		discard;

	// Light calculations
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	// Ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse lighting
	vec3 normal = normalize(f_normal);
	vec3 lightDir = normalize(lightPos - f_position);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular lighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - f_position);
	vec3 reflectDir = reflect(-lightDir, f_normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// Final light calculation
	vec3 result = ambient + diffuse + specular;

	vec3 mapped_normal = f_normal * 0.5 + 0.5; // Map normal range from [-1, 1] to [0, 1]

	if (showNormal) {
		FragColor = vec4(mapped_normal, 1.0);
	} else {
		FragColor = texture(objectTexture, f_uv) * vec4(result, 1.0);
	}
}