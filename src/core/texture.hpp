#pragma once
#include "util.hpp"
#include <string>
namespace renderme
{
	struct Texture: Only_Movable
	{
		unsinged int id;
		std::string type;
		std::string path;
	};
}