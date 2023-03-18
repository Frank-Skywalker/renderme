#pragma once
#include "shader.hpp"

namespace renderme
{

	struct Material
	{
		Material(std::string name);
		virtual auto gl_draw(Shader const& shader) const noexcept ->void = 0;
		std::string name;
	};
}