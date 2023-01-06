#pragma once
#include "type.hpp"

#include <glm/glm-all.hpp>

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

		glm::mat4 m;
		glm::mat4 minv;
	};

	//auto translate(Vector3f const& delta)->Transform;
	//auto translate(Float x, Float y, Float z)->Transform;
	//auto scale(Vector3f const& _scale)->Transform;
	//auto scale(Float x, Float y, Float z)->Transform;
	//auto rotate_x(Float theta)->Transform;
	//auto rotate_y(Float theta)->Transform;
	//auto rotate_z(Float theta)->Transform;
	//auto rotate(Vector3f const& axis, Float theta)->Transform;
	//auto lookat(Point3f const& position, Point3f const& lookat, Vector3f const& up)->Transform;
	//auto orthographic(Float znear, Float zfar)->Transform;
	//auto perspective(Float fov, Float znear, Float zfar)->Transform;
}