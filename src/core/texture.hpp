#pragma once
#include "util.hpp"
#include "runtime-path.hpp"
#include "shader.hpp"

#include <string>
namespace renderme
{
	enum struct Texture_Type
	{
		diffuse,
		specular,
		normal,
		ambient,
	};
	struct Texture final
	{
		Texture(Runtime_Path path, Texture_Type type);
		auto gl_draw(Shader const& shader, unsigned int i, unsigned int type_cnt) const noexcept -> void;

		unsigned int id;
		unsigned int format;
		int width;
		int height;
		int num_components;
		Texture_Type type;
		//std::string type;
		//std::string path;
	};
}