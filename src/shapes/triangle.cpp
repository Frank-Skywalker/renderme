#include "triangle.hpp"

#include <core/log.hpp>

#include <GL/glew.h>
#include <imgui/imgui.h>

namespace renderme
{
	Triangle_Mesh::Triangle_Mesh(Transform const* _object_to_world, Transform const* _world_to_object,
		std::vector<glm::uvec3> _faces, std::vector<glm::vec3> _positions, std::vector<glm::vec3> _normals,
		std::vector<glm::vec2> _uvs, std::vector<glm::vec3> _tangents, std::vector<glm::vec3> _bitangents)
		:Shape(_object_to_world, _world_to_object),
		faces{ std::move(_faces) }, positions{ std::move(_positions) }, normals{ std::move(_normals) },
		uvs{ std::move(_uvs) }, tangents{ std::move(_tangents) }, bitangents{ std::move(_bitangents) }
	{
		num_faces = faces.size();

		// create buffers/arrays
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;

		/////Vertex/////
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

		/////Normal/////
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

		/////UV/////
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

		/////EBO/////
		unsigned int ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(glm::uvec3), faces.data(), GL_STATIC_DRAW);

		//Unbind vao
		glBindVertexArray(0);
	}



	auto Triangle_Mesh::gl_draw(Shader const& shader) const noexcept -> void
	{
		shader.set_uniform_mat4("model", glm::identity<glm::mat4>());
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, num_faces * 3, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	auto Triangle_Mesh::intersect(Ray const& ray, Interaction* interaction, float* t) const noexcept ->bool
	{
		return false;
	}

	auto Triangle_Mesh::intersect_shadow(Ray const& ray) const noexcept ->bool
	{
		return false;
	}

	auto Triangle_Mesh::imgui_config() const noexcept ->void
	{
		ImGui::Text("Number of vertices: %d", positions.size());
		ImGui::Text("Number of faces: %d", faces.size());
	}



	auto Triangle_Mesh::create_triangles() const noexcept->std::vector<Triangle>
	{
		std::vector<Triangle> triangles;
		for (auto i = 0u; i < num_faces; ++i) {
			triangles.push_back(Triangle(_object_to_world, _world_to_object, this, i));
		}
		return triangles;
	}

	auto Triangle_Mesh::uvw_of(glm::vec3 point) const noexcept -> glm::vec3
	{
		return glm::vec3();
	}

	auto Triangle_Mesh::position_of(glm::vec3 uv) const noexcept -> glm::vec3
	{
		return glm::vec3();
	}

	auto Triangle_Mesh::normal_of(glm::vec3 uv) const noexcept -> glm::vec3
	{
		return glm::vec3();
	}

	auto Triangle_Mesh::surface_area() const noexcept -> float
	{
		return 0;
	}

	auto Triangle_Mesh::texture_coordinate_of(glm::vec3 uvw) const noexcept -> glm::vec2
	{
		return glm::vec2();
	}



	Triangle::Triangle(Transform const* object_to_world, Transform const* world_to_object, Triangle_Mesh const* mesh, unsigned int index)
		:Shape(object_to_world, world_to_object), mesh{ mesh }, index{ index }
	{
		auto& p0 = mesh->positions[mesh->faces[index].x];
		auto& p1 = mesh->positions[mesh->faces[index].y];
		auto& p2 = mesh->positions[mesh->faces[index].z];

		_object_bounds.eat(p0);
		_object_bounds.eat(p1);
		_object_bounds.eat(p2);

		if (_object_bounds.is_line()) {
			log(Status::error, "Line found");
		}

		if (object_to_world != nullptr) {
			_world_bounds.eat(object_to_world->transform_point(p0));
			_world_bounds.eat(object_to_world->transform_point(p1));
			_world_bounds.eat(object_to_world->transform_point(p2));
		}
		else {
			_world_bounds = _object_bounds;
		}

		// Calculate barycentric matrix
		barycentric_mat = glm::inverse(glm::mat3(p0, p1, p2));

		// Calculate surface area
		auto edge1 = p1 - p0;
		auto edge2 = p2 - p0;
		_surface_area = glm::length(glm::cross(edge1, edge2)) / 2.f;

		// Calculate override normal
		override_normal = glm::cross(edge1, edge2);
	}

	auto Triangle::gl_draw(Shader const& shader) const noexcept -> void
	{
		// Draw nothing
		return;
	}

	auto Triangle::intersect(Ray const& ray, Interaction* interaction, float* t_hit) const noexcept ->bool
	{
		// Get three points
		auto a = mesh->positions[mesh->faces[index].x];
		auto b = mesh->positions[mesh->faces[index].y];
		auto c = mesh->positions[mesh->faces[index].z];

		auto ro = ray.origin;
		auto rd = ray.direction;

		auto v1 = glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z);
		auto v2 = glm::vec3(a.x - c.x, a.y - c.y, a.z - c.z);
		auto v3 = glm::vec3(rd.x, rd.y, rd.z);

		float det_a = glm::determinant(glm::mat3(v1, v2, v3));

		v1 = glm::vec3(a.x - ro.x, a.y - ro.y, a.z - ro.z);
		v2 = glm::vec3(a.x - c.x, a.y - c.y, a.z - c.z);
		v3 = glm::vec3(rd.x, rd.y, rd.z);

		float det_beta = glm::determinant(glm::mat3(v1, v2, v3));

		v1 = glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z);
		v2 = glm::vec3(a.x - ro.x, a.y - ro.y, a.z - ro.z);
		v3 = glm::vec3(rd.x, rd.y, rd.z);

		float det_gamma = glm::determinant(glm::mat3(v1, v2, v3));

		v1 = glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z);
		v2 = glm::vec3(a.x - c.x, a.y - c.y, a.z - c.z);
		v3 = glm::vec3(a.x - ro.x, a.y - ro.y, a.z - ro.z);

		float det_t = glm::determinant(glm::mat3(v1, v2, v3));

		auto beta = det_beta / det_a;
		auto gamma = det_gamma / det_a;
		auto t = det_t / det_a;

		if (ray.is_valid(t) && beta > 0 && gamma > 0 && beta + gamma < 1)
		{
			*t_hit = t;
			interaction->position = ray.point_at(t);
			auto uvw = uvw_of(interaction->position);
			interaction->normal = normal_of(uvw);
			interaction->uv = texture_coordinate_of(uvw);
			return true;
		}

		return false;
	}

	auto Triangle::intersect_shadow(Ray const& ray) const noexcept ->bool
	{
		// Get three points
		auto a = mesh->positions[mesh->faces[index].x];
		auto b = mesh->positions[mesh->faces[index].y];
		auto c = mesh->positions[mesh->faces[index].z];

		auto ro = ray.origin;
		auto rd = ray.direction;

		auto v1 = glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z);
		auto v2 = glm::vec3(a.x - c.x, a.y - c.y, a.z - c.z);
		auto v3 = glm::vec3(rd.x, rd.y, rd.z);

		float det_a = glm::determinant(glm::mat3(v1, v2, v3));

		v1 = glm::vec3(a.x - ro.x, a.y - ro.y, a.z - ro.z);
		v2 = glm::vec3(a.x - c.x, a.y - c.y, a.z - c.z);
		v3 = glm::vec3(rd.x, rd.y, rd.z);

		float det_beta = glm::determinant(glm::mat3(v1, v2, v3));

		v1 = glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z);
		v2 = glm::vec3(a.x - ro.x, a.y - ro.y, a.z - ro.z);
		v3 = glm::vec3(rd.x, rd.y, rd.z);

		float det_gamma = glm::determinant(glm::mat3(v1, v2, v3));

		v1 = glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z);
		v2 = glm::vec3(a.x - c.x, a.y - c.y, a.z - c.z);
		v3 = glm::vec3(a.x - ro.x, a.y - ro.y, a.z - ro.z);

		float det_t = glm::determinant(glm::mat3(v1, v2, v3));

		auto beta = det_beta / det_a;
		auto gamma = det_gamma / det_a;
		auto t = det_t / det_a;

		if (ray.is_valid(t) && beta > 0 && gamma > 0 && beta + gamma < 1)
		{
			return true;
		}
		return false;
	}

	auto Triangle::imgui_config() const noexcept ->void
	{

	}

	auto Triangle::uvw_of(glm::vec3 point) const noexcept -> glm::vec3
	{
		return barycentric_mat * point;
	}

	auto Triangle::position_of(glm::vec3 uvw) const noexcept -> glm::vec3
	{
		return uvw.x * (mesh->positions[mesh->faces[index].x]) +
			uvw.y * (mesh->positions[mesh->faces[index].y]) +
			uvw.z * (mesh->positions[mesh->faces[index].z]);
	}

	auto Triangle::normal_of(glm::vec3 uvw) const noexcept -> glm::vec3
	{
		return override_normal;
		//return glm::normalize(
		//	uvw.x * (mesh->normals[mesh->faces[index].x]) +
		//	uvw.y * (mesh->normals[mesh->faces[index].y]) +
		//	uvw.z * (mesh->normals[mesh->faces[index].z])
		//);
	}

	auto Triangle::texture_coordinate_of(glm::vec3 uvw) const noexcept -> glm::vec2
	{
		return uvw.x * (mesh->uvs[mesh->faces[index].x]) +
			uvw.y * (mesh->uvs[mesh->faces[index].y]) +
			uvw.z * (mesh->uvs[mesh->faces[index].z]);
	}

	auto Triangle::surface_area() const noexcept -> float
	{
		return _surface_area;
	}

}