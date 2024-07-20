#include "engine/Shader.hpp"
#include "utils/FileSystem.hpp"
#include <iostream>

Shader::Shader(const std::string vertexFilePath, const std::string fragmentFilePath)
{
	std::string vertexSource;
	std::string fragmentSource;

	try
	{
		vertexSource = FileSystem::read(vertexFilePath);
		fragmentSource = FileSystem::read(fragmentFilePath);
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	const char *vertexSourceC = vertexSource.c_str();
	const char *fragmentSourceC = fragmentSource.c_str();

	unsigned int vertexShader, fragmentShader;

	// Compile vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSourceC, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
	}

	// Compile fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSourceC, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
	}

	// Link shaders
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cerr << "Shader program linking failed: " << infoLog << std::endl;
	}

	// Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}