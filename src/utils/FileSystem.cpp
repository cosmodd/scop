#include "utils/FileSystem.hpp"

namespace FileSystem
{

	std::string read(const std::string path)
	{
		std::string content;
		std::ifstream file(path, std::ios::binary | std::ios::in | std::ios::ate);

		if (!file.is_open())
			throw std::runtime_error("Failed to open file: " + path);

		content.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(&content[0], content.size());
		file.close();

		return content;
	}

}