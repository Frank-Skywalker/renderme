#pragma once

#include "shader.hpp"

namespace renderme
{
	struct Shape
	{
		virtual auto gl_draw(Shader const& shader) const noexcept -> void = 0;
		virtual auto intersect() const noexcept ->bool = 0;
		virtual auto intersect_shadow() const noexcept ->bool = 0;
	};
}