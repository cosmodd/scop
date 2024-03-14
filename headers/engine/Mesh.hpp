#pragma once

#include <vector>
#include <glad/glad.h>

namespace engine {

	class Mesh {

		private:
			unsigned int vao, vbo, ebo;
			unsigned int indicesCount;

		public:
			Mesh(
				const std::vector<float>& vertices,
				const std::vector<unsigned int>& indices
			);
			~Mesh();

			void draw() const;
	};

	Mesh* loadObject(const char* path);

}