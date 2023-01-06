#include "triangle.hpp"

#include <core/log.hpp>

#include <GL/glew.h>

namespace renderme
{
	Triangle_Mesh::Triangle_Mesh(Transform const* _object_to_world, Transform const* _world_to_object,
		std::vector<glm::uvec3> _faces, std::vector<glm::vec3> _positions, std::vector<glm::vec3> _normals,
		std::vector<glm::vec2> _uvs, std::vector<glm::vec3> _tangents, std::vector<glm::vec3> _bitangents)
		:Shape(_object_to_world, _world_to_object),
		faces{std::move(_faces)}, positions{std::move(_positions)}, normals{std::move(_normals)},
		uvs{std::move(_uvs)}, tangents{std::move(_tangents)}, bitangents{std::move(_bitangents)}
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
		glDrawElements(GL_TRIANGLES, num_faces*3 , GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	auto Triangle_Mesh::intersect() const noexcept ->bool
	{
		//intersect nothing
		return false;
	}

	auto Triangle_Mesh::intersect_shadow() const noexcept ->bool
	{
		//intersect nothing
		return false;
	}


	auto Triangle_Mesh::create_triangles() const noexcept->std::vector<Triangle>
	{
		std::vector<Triangle> triangles;
		for (auto i = 0u; i < num_faces; ++i) {
			triangles.push_back(Triangle(object_to_world, world_to_object, this, i));
		}
		return triangles;
	}


	Triangle::Triangle(Transform const* object_to_world, Transform const* world_to_object, Triangle_Mesh const* mesh, unsigned int index)
		:Shape(object_to_world, world_to_object), mesh{mesh}, index{index}
	{}

	auto Triangle::gl_draw(Shader const& shader) const noexcept -> void
	{
		//draw nothing
		return;
	}

	auto Triangle::intersect() const noexcept ->bool
	{
		return true;
	}

	auto Triangle::intersect_shadow() const noexcept ->bool
	{
		return true;
	}


}