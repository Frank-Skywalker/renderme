#pragma once
#include <glm/glm-all.hpp>

namespace renderme
{
	struct Interaction final
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};
}