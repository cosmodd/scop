#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
	public:
		unsigned int ID;

		Shader(const std::string vertexFilePath, const std::string fragmentFilePath);

		void use();

		// Uniforms
		void setBool(const std::string name, bool value) const;
		void setInt(const std::string name, int value) const;
		void setFloat(const std::string name, float value) const;
};