#include"primitive.hpp"
#include "GL/glew.h"
namespace renderme
{
	Shape_Primitive::Shape_Primitive(Shape* shape)
	{
		this->shape.reset(shape);
	}

	auto Shape_Primitive::gl_draw(Shader const& shader) const noexcept -> void
	{
		shape->gl_draw(shader);
	}

	auto Shape_Primitive::intersect() const noexcept ->bool
	{
		return shape->intersect();
	}

	auto Shape_Primitive::intersect_shadow() const noexcept ->bool
	{
		return shape->intersect_shadow();
	}


	Mesh::Mesh(std::vector<Point3f> vertices, std::vector<Normal3f> normals, std::vector<Vector2f> uvs, std::vector<unsigned int> indices)
		:vertices{std::move(vertices)}, normals{std::move(normals)}, uvs{std::move(uvs)}, indices{std::move(indices)}
	{
		// create buffers/arrays
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;

		/////Vertex/////
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point3f), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point3f), nullptr);

		/////Normal/////
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Normal3f), normals.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point3f), nullptr);

		/////UV/////
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2f), uvs.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), nullptr);

		//Unbind vbo
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/////EBO/////
		unsigned int ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		//Unbind ebo
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//Unbind vao
		glBindVertexArray(0);
	}



	auto Mesh::gl_draw(Shader const& shader) const noexcept -> void
	{
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	auto Mesh::intersect() const noexcept ->bool
	{
		return true;
	}

	auto Mesh::intersect_shadow() const noexcept ->bool
	{
		return true;
	}

}