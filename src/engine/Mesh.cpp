#include "engine/Mesh.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

Mesh::Mesh() : VAO(0), VBO(0), EBO(0), boundingBox(), center(), size() {}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : vertices(vertices),
																			  indices(indices)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Vertex Attributes
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(0);
	// UV coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(1);
	// Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	boundingBox.min = Vec3(std::numeric_limits<float>::max());
	boundingBox.max = Vec3(std::numeric_limits<float>::min());

	for (const Vertex &vertex : vertices)
	{
		if (vertex.position.x < boundingBox.min.x)
			boundingBox.min.x = vertex.position.x;
		if (vertex.position.y < boundingBox.min.y)
			boundingBox.min.y = vertex.position.y;
		if (vertex.position.z < boundingBox.min.z)
			boundingBox.min.z = vertex.position.z;

		if (vertex.position.x > boundingBox.max.x)
			boundingBox.max.x = vertex.position.x;
		if (vertex.position.y > boundingBox.max.y)
			boundingBox.max.y = vertex.position.y;
		if (vertex.position.z > boundingBox.max.z)
			boundingBox.max.z = vertex.position.z;
	}

	this->center = (boundingBox.min + boundingBox.max) / 2.0f;
	this->size = (boundingBox.max - boundingBox.min).magnitude();
}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh loadMesh(const std::string &path)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<Vec3> positions;
	std::vector<Vec2> uvs;
	std::vector<Vec3> normals;
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

	std::ifstream file(path);

	if (!file.is_open())
		throw std::runtime_error("Failed to open file: " + path);

	std::string line;

	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string type;
		stream >> type;

		if (type == "v")
		{
			Vec3 position;
			stream >> position.x >> position.y >> position.z;
			positions.push_back(position);
		}
		else if (type == "vt")
		{
			Vec2 uv;
			stream >> uv.x >> uv.y;
			uvs.push_back(uv);
		}
		else if (type == "vn")
		{
			Vec3 normal;
			stream >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (type == "f")
		{
			std::string vertex;
			uint32_t indices[4][3] = {{0}};
			int indexCount = 0;

			while (stream >> vertex && indexCount < 4)
			{
				std::istringstream vertexStream(vertex);
				std::string index;

				for (int j = 0; j < 3 && std::getline(vertexStream, index, '/'); j++)
				{
					if (index.empty())
						continue;

					const unsigned int indexValue = std::stoi(index);
					indices[indexCount][j] = indexValue;
				}

				indexCount++;
			}

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					const uint32_t indexValue = indices[i][j] - 1;

					if (indices[i][j] == 0)
						continue;

					if (j == 0)
						vertexIndices.push_back(indexValue);
					else if (j == 1)
						uvIndices.push_back(indexValue);
					else if (j == 2)
						normalIndices.push_back(indexValue);
				}
			}

			if (indexCount > 3)
			{
				for (int i = 0; i < 3; i++)
				{
					const int index = (i + 2) % 4;
					for (int j = 0; j < 3; j++)
					{
						const uint32_t indexValue = indices[index][j] - 1;

						if (indices[index][j] == 0)
							continue;

						if (j == 0)
							vertexIndices.push_back(indexValue);
						else if (j == 1)
							uvIndices.push_back(indexValue);
						else if (j == 2)
							normalIndices.push_back(indexValue);
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		Vertex vertex;

		vertex.position = positions[vertexIndices[i]];

		if (!uvs.empty() && i < uvIndices.size())
			vertex.texCoords = uvs[uvIndices[i]];

		if (!normals.empty() && i < normalIndices.size())
			vertex.normal = normals[normalIndices[i]];

		vertices.push_back(vertex);
		indices.push_back(i);
	}

	// Generate normals if they doesn't exist
	if (normals.empty())
	{
		for (uint32_t i = 0; i < indices.size(); i += 3)
		{
			Vec3 a = vertices[indices[i]].position;
			Vec3 b = vertices[indices[i + 1]].position;
			Vec3 c = vertices[indices[i + 2]].position;

			Vec3 u = b - a;
			Vec3 v = c - a;

			Vec3 normal = u.cross(v);
			normal.normalize();

			vertices[indices[i]].normal = normal;
			vertices[indices[i + 1]].normal = normal;
			vertices[indices[i + 2]].normal = normal;
		}
	}

	return Mesh(vertices, indices);
}