#pragma once

#include <core/shape.hpp>
#include <core/geometry.hpp>

#include <vector>

struct Triangle_Mesh
{
	std::vector<unsigned int> indices;
	std::vector<Point3f> vertices;
	std::vector<Normal3f> normals;
	std::vector<Vector2f> uvs;
};

struct Triangle: Shape
{

};