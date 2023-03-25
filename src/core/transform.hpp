#pragma once

#include <glm/glm-all.hpp>
#include "ray.hpp"
#include <iostream>

namespace renderme
{
	struct Transform final
	{
		Transform();
		Transform(glm::mat4 const& _m);
		Transform(glm::mat4 const& _m, glm::mat4 const& _minv);

		auto is_identity() const noexcept -> bool;
		auto transpose() const noexcept->Transform;
		auto inverse() const noexcept->Transform;


		auto operator*(Transform const& rhs) const noexcept -> Transform;
		auto operator==(Transform const& rhs) const noexcept->bool;
		auto operator!=(Transform const& rhs) const noexcept->bool;

		friend auto operator<<(std::ostream& os, Transform const& transform)->std::ostream&;

		template<typename T>
		auto transform_point(glm::tvec3<T> const& point) const noexcept ->glm::tvec3<T>;
		template<typename T>
		auto transform_vector(glm::tvec3<T> const& vector) const noexcept ->glm::tvec3<T>;
		template<typename T>
		auto transform_normal(glm::tvec3<T> const& normal) const noexcept ->glm::tvec3<T>;

		auto transform_ray(Ray const& ray) const noexcept ->Ray;


		glm::mat4 m;
		glm::mat4 minv;
	};

	//auto translate(Vector3f const& delta)->Transform;
	//auto translate(float x, float y, float z)->Transform;
	//auto scale(Vector3f const& _scale)->Transform;
	//auto scale(float x, float y, float z)->Transform;
	//auto rotate_x(float theta)->Transform;
	//auto rotate_y(float theta)->Transform;
	//auto rotate_z(float theta)->Transform;
	//auto rotate(Vector3f const& axis, float theta)->Transform;
	//auto lookat(Point3f const& position, Point3f const& lookat, Vector3f const& up)->Transform;
	//auto orthographic(float znear, float zfar)->Transform;
	//auto perspective(float fov, float znear, float zfar)->Transform;
}

#include "transform.inl"
