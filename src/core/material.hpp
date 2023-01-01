#pragma once
#include "shader.hpp"

namespace renderme
{

	struct Material
	{
		virtual auto gl_draw(Shader const& shader) const noexcept ->void = 0;
	};
}