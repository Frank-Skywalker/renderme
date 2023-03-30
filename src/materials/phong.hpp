#pragma once

#include <core/material.hpp>
#include<core/texture.hpp>

#include<array>

namespace renderme
{
	struct Phong_Material final : Material
	{
		Phong_Material(std::string name);
		auto gl_draw(Shader const& shader) const noexcept ->void;


		auto ambient(glm::vec2 uv) const noexcept ->glm::vec3 override;
		auto diffuse(glm::vec2 uv) const noexcept ->glm::vec3 override;
		auto specular(glm::vec2 uv) const noexcept ->glm::vec3 override;
		auto specular_exponent(glm::vec2 uv) const noexcept ->float override;
		auto transparent(glm::vec2 uv) const noexcept ->glm::vec3 override;
		auto refraction_index(glm::vec2 uv) const noexcept ->float override;
		auto emition(glm::vec2 uv) const noexcept ->glm::vec3 override;

		//glm::vec3 ambient;								// Ka
		glm::vec3 _diffuse{ 0.f,0.f,0.f };					// Kd
		glm::vec3 _specular{ 0.f,0.f,0.f };					// Ks
		float _specular_exponent;							// Ns
		glm::vec3 _transparent{ 0.f,0.f,0.f };				// Tr
		float _refraction_index;							// Ni
		glm::vec3 _emition{ 0.f,0.f,0.f };					// Ke

		std::array<Texture*, size_t(Texture_Type::invalid)> textures;
	};
}
