#pragma once

#include <core/material.hpp>
#include<core/texture.hpp>

#include<vector>

namespace renderme
{
	struct Phong_Material final: Material
	{
		auto gl_draw(Shader const& shader) const noexcept ->void;
		std::vector<Texture> textures;
	};
}