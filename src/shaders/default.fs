#version 420

out vec4 FragColor;

in vec3 f_position;
in vec2 f_uv;
in vec3 f_normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D objectTexture;

void main()
{
	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;

	// Light calculations
	vec3 lightDir = normalize(lightPos - f_position);
	vec3 viewDir = normalize(viewPos - f_position);
	vec3 reflectDir = reflect(-lightDir, f_normal);

	// Diffuse lighting
	vec3 normal = normalize(f_normal);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);

	// Specular lighting
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = specularStrength * spec * vec3(1.0f, 1.0f, 1.0f);

	// Final light calculation
	vec3 result = ambientStrength + diffuse + specular;

	// FragColor = vec4(result, 1.0f);
	FragColor = texture(objectTexture, f_uv) * vec4(result, 1.0f);
}