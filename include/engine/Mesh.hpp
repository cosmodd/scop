#pragma once
#include <vector>

#include "maths/Vec2.hpp"
#include "maths/Vec3.hpp"
#include "engine/Texture.hpp"

struct Vertex
{
	Vec3 position;
	Vec2 texCoords;
	Vec3 normal;
};

struct BoundingBox
{
	Vec3 min, max;
};

class Mesh
{
private:
	unsigned int VAO, VBO, EBO;

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	BoundingBox boundingBox;
	Vec3 center;
	float size;

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	void draw();
};

Mesh loadMesh(const std::string &path);