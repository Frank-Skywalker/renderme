#pragma once
#include "util.hpp"
#include "runtime-path.hpp"
#include "shader.hpp"

#include <string>
namespace renderme
{
	enum struct Texture_Type: uint16_t
	{
		ambient,		// map_Ka
		diffuse,		// map_Kd
		specular,		// map_Ks
		normal,
		invalid,
	};
	struct Texture final
	{
		Texture(Texture_Type type, Runtime_Path path);
		Texture(Texture_Type type, glm::vec3 color);
		auto gl_draw(Shader const& shader, unsigned int i, unsigned int type_cnt) const noexcept -> void;
		auto color_of(glm::vec2 uv) const noexcept -> glm::vec3;

		Texture_Type type;
		unsigned int id;
		unsigned int format;
		int num_components;
		int width;
		int height;

		std::unique_ptr<glm::vec3[]> pixels;

	};
}