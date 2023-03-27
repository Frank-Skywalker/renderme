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

		//glm::vec3 ambient;								// Ka
		glm::vec3 diffuse{ 0.f,0.f,0.f };					// Kd
		glm::vec3 specular{ 0.f,0.f,0.f };					// Ks
		float specular_exponent;							// Ns
		glm::vec3 transparent{ 0.f,0.f,0.f };				// Tr
		float refraction_index;								// Ni
		glm::vec3 emition{ 0.f,0.f,0.f };					// Ke
		std::vector<Texture*> textures;
	};
}
