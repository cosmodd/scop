#version 420

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D objectTexture;

void main()
{
	// FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	FragColor = texture(objectTexture, texCoord);
}