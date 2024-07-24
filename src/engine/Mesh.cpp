#include "engine/Mesh.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices):
	vertices(vertices),
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
}

BoundingBox Mesh::getBoundingBox()
{
	BoundingBox boundingBox;

	boundingBox.min = Vec3(std::numeric_limits<float>::max());
	boundingBox.max = Vec3(std::numeric_limits<float>::min());

	for (const Vertex& vertex : vertices)
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

	return boundingBox;
}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh loadMesh(const std::string& path)
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

			if (std::count(line.begin(), line.end(), ' ') >= 4)
				throw std::runtime_error("Only models that are triangulated are supported");

			while (stream >> vertex)
			{
				std::istringstream vertexStream(vertex);
				std::string index;

				for (int j = 0; j < 3 && std::getline(vertexStream, index, '/'); j++)
				{
					if (index.empty())
						continue;

					const unsigned int indexValue = std::stoi(index) - 1;

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

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		Vertex vertex;

		vertex.position = positions[vertexIndices[i]];

		if (!uvs.empty())
			vertex.texCoords = uvs[uvIndices[i]];

		if (!normals.empty())
			vertex.normal = normals[normalIndices[i]];

		vertices.push_back(vertex);
		indices.push_back(i);
	}

	return Mesh(vertices, indices);
}