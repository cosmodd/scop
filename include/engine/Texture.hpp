#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "stb_image.h"

class Texture
{
	private:
		unsigned int id;

	public:
		Texture(const std::string& path);
		~Texture();

		void bind(unsigned int slot = 0) const;
};