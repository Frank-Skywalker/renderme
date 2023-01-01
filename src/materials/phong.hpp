#pragma once

#include <core/material.hpp>
#include<core/texture.hpp>

#include<vector>

namespace renderme
{
	struct Phong_Material final: Material
	{
		Phong_Material(std::vector<std::shared_ptr<Texture>> texs);
		auto gl_draw(Shader const& shader) const noexcept ->void;
		std::vector<std::shared_ptr<Texture>> textures;
	};
}