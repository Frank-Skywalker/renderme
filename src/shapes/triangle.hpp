#pragma once

#include <core/shape.hpp>
#include <core/primitive.hpp>

#include <glm/glm-all.hpp>

#include<vector>

namespace renderme
{
	struct Triangle;

	//WARNING: If a triangle mesh is moved in memory, subsequent triangles will fail to refer to its mesh using pointer
	struct Triangle_Mesh final: Shape
	{
		Triangle_Mesh(Transform const* _object_to_world, Transform const* _world_to_object,
			std::vector<glm::uvec3> _faces, std::vector<glm::vec3> _positions, std::vector<glm::vec3> _normals,
			std::vector<glm::vec2> _uvs, std::vector<glm::vec3> _tangents, std::vector<glm::vec3> _bitangents);
		~Triangle_Mesh() = default;
		Triangle_Mesh(Triangle_Mesh const&) = default;
		auto operator=(Triangle_Mesh const&)->Triangle_Mesh & = default;
		Triangle_Mesh(Triangle_Mesh&&) = default;
		auto operator=(Triangle_Mesh&&)->Triangle_Mesh & = default;

		auto gl_draw(Shader const& shader) const noexcept -> void final;
		auto intersect(Ray const& ray, Interaction* interaction, float* t) const noexcept ->bool final;
		auto intersect_shadow(Ray const& ray) const noexcept ->bool final;
		auto imgui_config() const noexcept ->void final;

		auto surface_area() const noexcept -> float override;
		auto uvw_of(glm::vec3 point) const noexcept -> glm::vec3 override;
		auto position_of(glm::vec3 uvw) const noexcept -> glm::vec3 override;
		auto normal_of(glm::vec3 uvw) const noexcept -> glm::vec3 override;
		auto texture_coordinate_of(glm::vec3 uvw) const noexcept -> glm::vec2 override;

		auto create_triangles() const noexcept->std::vector<Triangle>;

		unsigned int num_faces;
		std::vector<glm::uvec3> faces;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;

	private:
		unsigned int vao;
	};


	struct Triangle final: Shape
	{
		Triangle(Transform const* object_to_world, Transform const* world_to_object, Triangle_Mesh const* mesh, unsigned int index);
		auto gl_draw(Shader const& shader) const noexcept -> void final;
		auto intersect(Ray const& ray, Interaction* interaction, float* t_hit) const noexcept ->bool final;
		auto intersect_shadow(Ray const& ray) const noexcept ->bool final;
		auto imgui_config() const noexcept ->void final;

		auto surface_area() const noexcept -> float override;
		auto uvw_of(glm::vec3 point) const noexcept -> glm::vec3 override;
		auto position_of(glm::vec3 uvw) const noexcept -> glm::vec3 override;
		auto normal_of(glm::vec3 uvw) const noexcept -> glm::vec3 override;
		auto texture_coordinate_of(glm::vec3 uvw) const noexcept -> glm::vec2 override;

	private:
		Triangle_Mesh const* mesh;
		unsigned int index;
		glm::mat3 barycentric_mat;
		float _surface_area;
	};
}
