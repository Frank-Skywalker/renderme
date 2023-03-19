#pragma once
#include "material.hpp"

namespace renderme
{
	struct Light
	{
		Material* material;
		glm::vec3 radiance;
	};
}