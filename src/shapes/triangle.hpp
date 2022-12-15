#pragma once

#include <core/shape.hpp>
#include <core/geometry.hpp>

namespace renderme
{

	struct Triangle final: Shape
	{
		auto gl_draw(Shader const& shader) const noexcept -> void;
		auto intersect() const noexcept ->bool;
		auto intersect_shadow() const noexcept ->bool;
	};
}
