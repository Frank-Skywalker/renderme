#pragma once

#include <core/shape.hpp>
#include <core/vector.hpp>
#include <core/primitive.hpp>

#include<vector>

namespace renderme
{
	struct Triangle;

	//WARNING: If a triangle mesh is moved in memory, subsequent triangles will fail to refer to its mesh using pointer
	struct Triangle_Mesh final: Shape
	{
		Triangle_Mesh(Transform const* _object_to_world, Transform const* _world_to_object,
			std::vector<Point3ui> _faces, std::vector<Point3f> _positions, std::vector<Normal3f> _normals,
			std::vector<Point2f> _uvs, std::vector<Vector3f> _tangents, std::vector<Vector3f> _bitangents);
		~Triangle_Mesh() = default;
		Triangle_Mesh(Triangle_Mesh const&) = default;
		auto operator=(Triangle_Mesh const&)->Triangle_Mesh & = default;
		Triangle_Mesh(Triangle_Mesh&&) = default;
		auto operator=(Triangle_Mesh&&)->Triangle_Mesh & = default;

		auto gl_draw(Shader const& shader) const noexcept -> void;
		auto intersect() const noexcept ->bool;
		auto intersect_shadow() const noexcept ->bool;

		auto create_triangles() const noexcept->std::vector<Triangle>;

		unsigned int num_faces;
		std::vector<Point3ui> faces;
		std::vector<Point3f> positions;
		std::vector<Normal3f> normals;
		std::vector<Point2f> uvs;
		std::vector<Vector3f> tangents;
		std::vector<Vector3f> bitangents;

	private:
		unsigned int vao;
	};


	struct Triangle final: Shape
	{
		Triangle(Transform const* object_to_world, Transform const* world_to_object, Triangle_Mesh const* mesh, unsigned int index);
		auto gl_draw(Shader const& shader) const noexcept -> void;
		auto intersect() const noexcept ->bool;
		auto intersect_shadow() const noexcept ->bool;

		Triangle_Mesh const* mesh;
		unsigned int index;
	};
}
