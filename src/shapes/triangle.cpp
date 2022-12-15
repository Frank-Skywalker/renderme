#include "triangle.hpp"

namespace renderme
{
	Triangle::Triangle(Transform const* object_to_world, Transform const* world_to_object)
		:Shape{object_to_world, world_to_object}{}

	auto Triangle::gl_draw(Shader const& shader) const noexcept -> void
	{

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