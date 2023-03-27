#pragma once
#include "material.hpp"

#include <glm/glm-all.hpp>

namespace renderme
{
	struct Primitive;
	struct Interaction final
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color{ 0.f,0.f,0.f };
		Material const* material;
		Primitive const* primitive;
	};
}