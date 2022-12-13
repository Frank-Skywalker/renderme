#pragma once

#include <core/shape.hpp>
#include <core/geometry.hpp>

namespace renderme
{

	struct Triangle final: Shape
	{
		auto gl_draw() const noexcept ->void;
		auto intersect() const noexcept ->void;
		auto intersect_shadow() const noexcept ->void;
	};
}
