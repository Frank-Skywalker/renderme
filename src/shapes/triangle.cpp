#include "triangle.hpp"

namespace renderme
{


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