#include <fstream>
#include <iostream>
#include "engine/Mesh.hpp"
#include "maths/Vec3.hpp"
#include "maths/Vec2.hpp"

namespace engine {

	Mesh::Mesh(
		const std::vector<float>& vertices,
		const std::vector<unsigned int>& indices
	): indicesCount(indices.size()) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			vertices.size() * sizeof(float),
			vertices.data(),
			GL_STATIC_DRAW
		);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			indices.size() * sizeof(unsigned int),
			indices.data(),
			GL_STATIC_DRAW
		);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// UV
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Mesh::~Mesh() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}

	void Mesh::draw() const {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	}

	Mesh* loadObject(const char* path) {
		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<maths::Vec3> tempVertices;
		std::vector<maths::Vec2> tempUvs;
		std::vector<maths::Vec3> tempNormals;
		std::ifstream file(path);

		if (!file.is_open()) {
			std::cerr << "Could not open file " << path << std::endl;
			return nullptr;
		}

		std::string line;
		bool hasUvs = false, hasNormals = false;
		while (std::getline(file, line)) {
			std::string type = line.substr(0, line.find(' '));

			if (type == "v") {
				maths::Vec3 vertex;
				sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
				tempVertices.push_back(vertex);
			} else if (type == "vt") {
				maths::Vec2 uv;
				sscanf(line.c_str(), "vt %f %f", &uv.x, &uv.y);
				tempUvs.push_back(uv);
				hasUvs = true;
			} else if (type == "vn") {
				maths::Vec3 normal;
				sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
				tempNormals.push_back(normal);
				hasNormals = true;
			} else if (type == "f") {
				unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
				int matches = 0;

				if (hasUvs && hasNormals) {
					matches = sscanf(
						line.c_str(),
						"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
						&vertexIndex[0], &uvIndex[0], &normalIndex[0],
						&vertexIndex[1], &uvIndex[1], &normalIndex[1],
						&vertexIndex[2], &uvIndex[2], &normalIndex[2],
						&vertexIndex[3], &uvIndex[3], &normalIndex[3]
					);
				} else if (hasUvs) {
					matches = sscanf(
						line.c_str(),
						"f %d/%d %d/%d %d/%d %d/%d",
						&vertexIndex[0], &uvIndex[0],
						&vertexIndex[1], &uvIndex[1],
						&vertexIndex[2], &uvIndex[2],
						&vertexIndex[3], &uvIndex[3]
					);
				} else if (hasNormals) {
					matches = sscanf(
						line.c_str(),
						"f %d//%d %d//%d %d//%d %d//%d",
						&vertexIndex[0], &normalIndex[0],
						&vertexIndex[1], &normalIndex[1],
						&vertexIndex[2], &normalIndex[2],
						&vertexIndex[3], &normalIndex[3]
					);
				} else {
					matches = sscanf(
						line.c_str(),
						"f %d %d %d %d",
						&vertexIndex[0],
						&vertexIndex[1],
						&vertexIndex[2],
						&vertexIndex[3]
					);
				}

				unsigned int faceLength = matches / (1 + hasUvs + hasNormals);

				if (faceLength == 3) {
					for (int i = 0; i < 3; i++) {
						vertexIndices.push_back(vertexIndex[i] - 1);
						if (hasUvs) uvIndices.push_back(uvIndex[i] - 1);
						if (hasNormals) normalIndices.push_back(normalIndex[i] - 1);
					}
				} else if (faceLength == 4) {
					for (int i = 0; i < 3; i++) {
						vertexIndices.push_back(vertexIndex[i] - 1);
						if (hasUvs) uvIndices.push_back(uvIndex[i] - 1);
						if (hasNormals) normalIndices.push_back(normalIndex[i] - 1);
					}
					for (int i = 2; i < 4; i++) {
						vertexIndices.push_back(vertexIndex[i] - 1);
						if (hasUvs) uvIndices.push_back(uvIndex[i] - 1);
						if (hasNormals) normalIndices.push_back(normalIndex[i] - 1);
					}
					vertexIndices.push_back(vertexIndex[0] - 1);
					if (hasUvs) uvIndices.push_back(uvIndex[0] - 1);
					if (hasNormals) normalIndices.push_back(normalIndex[0] - 1);
				} else {
					std::cerr << "Face length not supported" << std::endl;
					return nullptr;
				}
			}
		}

		std::cout << "Vertices: " << tempVertices.size() << std::endl;
		std::cout << "UVs: " << tempUvs.size() << std::endl;
		std::cout << "Normals: " << tempNormals.size() << std::endl;
		std::cout << "Faces: " << vertexIndices.size() / 3 << std::endl;
		std::cout << "Vertex indices: " << vertexIndices.size() << std::endl;
		std::cout << "UV indices: " << uvIndices.size() << std::endl;
		std::cout << "Normal indices: " << normalIndices.size() << std::endl;


		std::vector<float> vertices;
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
			maths::Vec3 vertex = tempVertices[vertexIndices[i]];
			maths::Vec2 uv = {0, 0};
			maths::Vec3 normal = {0, 0, 0};

			vertices.push_back(vertex.x);
			vertices.push_back(vertex.y);
			vertices.push_back(vertex.z);
			if (hasUvs) uv = tempUvs[uvIndices[i]];
			vertices.push_back(uv.x);
			vertices.push_back(uv.y);
			if (hasNormals) normal = tempNormals[normalIndices[i]];
			vertices.push_back(normal.x);
			vertices.push_back(normal.y);
			vertices.push_back(normal.z);
		}

		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
			indices.push_back(i);
		}
		return new Mesh(vertices, indices);
	};

}