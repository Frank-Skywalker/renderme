#pragma once
#include "shader.hpp"
#include "texture.hpp"

namespace renderme
{

	struct Material
	{
		Material(std::string name);
		virtual auto gl_draw(Shader const& shader) const noexcept ->void;

		std::string name;

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