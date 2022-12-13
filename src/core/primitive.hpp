#pragma once
#include "util.hpp"
#include "shape.hpp"
#include "geometry.hpp"

#include<vector>
#include<memory>
namespace renderme
{
	struct Primitive : Only_Movable
	{
		virtual auto gl_draw() const noexcept ->void = 0;
		virtual auto intersect() const noexcept ->void = 0;
		virtual auto intersect_shadow() const noexcept ->void = 0;
	};

	struct Shape_Primitive final: Primitive
	{
		Shape_Primitive(Shape* shape);
		auto gl_draw() const noexcept ->void;
		auto intersect() const noexcept ->void;
		auto intersect_shadow() const noexcept ->void;

	private:
		std::unique_ptr<Shape> shape;
	};


	struct Mesh final: Primitive
	{
		Mesh(std::vector<Point3f> vertices, std::vector<Normal3f> normals, std::vector<Vector2f> uvs, std::vector<unsigned int> indices);
		~Mesh() = default;
		Mesh(Mesh const&) = default;
		auto operator=(Mesh const&)->Mesh & = default;
		Mesh(Mesh&&) = default;
		auto operator=(Mesh&&)->Mesh & = default;

		auto gl_draw() const noexcept ->void;
		auto intersect() const noexcept ->void;
		auto intersect_shadow() const noexcept ->void;

	private:
		unsigned int vao;

		std::vector<unsigned int> indices;
		std::vector<Point3f> vertices;
		std::vector<Normal3f> normals;
		std::vector<Vector2f> uvs;
	};


	struct Aggregate: Primitive
	{
	};
}