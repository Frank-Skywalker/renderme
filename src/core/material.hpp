#pragma once
#include "shader.hpp"

namespace renderme
{

	struct Material
	{
		Material(std::string name);
		virtual auto gl_draw(Shader const& shader) const noexcept ->void = 0;

		virtual auto diffuse(glm::vec2 uv) const noexcept ->glm::vec3 = 0;
		virtual auto specular(glm::vec2 uv) const noexcept ->glm::vec3 = 0;
		virtual auto specular_exponent(glm::vec2 uv) const noexcept ->float = 0;
		virtual auto transparent(glm::vec2 uv) const noexcept ->glm::vec3 = 0;
		virtual auto refraction_index(glm::vec2 uv) const noexcept ->float = 0;
		virtual auto emition(glm::vec2 uv) const noexcept ->glm::vec3 = 0;

		std::string name;

	};
}