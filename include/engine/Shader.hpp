#pragma once

#include <string>
#include <glad/glad.h>

#include "maths/Vec2.hpp"
#include "maths/Vec3.hpp"
#include "maths/Mat4.hpp"

class Shader
{
	public:
		unsigned int ID;

		Shader(const std::string vertexFilePath, const std::string fragmentFilePath);

		void use();

		// Uniforms
		void setBool(const std::string name, const bool value) const;
		void setInt(const std::string name, const int value) const;
		void setFloat(const std::string name, const float value) const;
		void setVec2(const std::string name, const Vec2& value) const;
		void setVec3(const std::string name, const Vec3& value) const;
		void setMat4(const std::string name, const Mat4& value) const;
};