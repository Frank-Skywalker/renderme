#pragma once
#include<glm/glm-all.hpp>

namespace renderme
{
	struct Ray final
	{
		Ray(glm::vec3 const& origin, glm::vec3 const& direction, float tmax = std::numeric_limits<float>::max());

		glm::vec3 origin;
		glm::vec3 direction;
		float tmax{ std::numeric_limits<float>::max() };
		glm::vec3 inv_direction;
	};
}