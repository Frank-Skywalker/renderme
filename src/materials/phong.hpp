#pragma once

#include <core/material.hpp>
#include<core/texture.hpp>

#include<vector>

namespace renderme
{
	struct Phong_Material final: Material
	{
		Phong_Material(std::string name);
		auto gl_draw(Shader const& shader) const noexcept ->void;

		//glm::vec3 ambient;				// Ka
		glm::vec3 diffuse;					// Kd
		glm::vec3 specular;					// Ks
		float specular_exponent;			// Ns
		glm::vec3 transparent;				// Tr
		float refraction_index;				// Ni
		std::vector<Texture*> textures;
	};
}
