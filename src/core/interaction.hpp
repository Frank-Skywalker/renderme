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
		glm::vec2 uv;
		Material const* material{nullptr};
		Primitive const* primitive{nullptr};
	};
}