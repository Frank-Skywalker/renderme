#pragma once
#include "transform.hpp"

namespace renderme
{
	template<typename T>
	auto Transform::transform_point(glm::tvec3<T> const& p) const noexcept ->glm::tvec3<T>
	{
		glm::tvec4<T> v(p, 1);
		v = m * v;
		if (v.w == 1)
			return glm::tvec3<T>(v);
		else
			return glm::tvec3<T>(v) / v.w;
	}

	template<typename T>
	auto Transform::transform_vector(glm::tvec3<T> const& vector) const noexcept ->glm::tvec3<T>
	{
		glm::tvec4<T> v(vector, 0);
		v = m * v;
		return glm::tvec3<T>(v);
	}

	template<typename T>
	auto Transform::transform_normal(glm::tvec3<T> const& n) const noexcept ->glm::tvec3<T>
	{
		glm::tvec4<T> v(n, 0);
		auto minv_trans = glm::transpose(minv);
		v = minv_trans * v;
		return glm::tvec3<T>(v);
	}
}