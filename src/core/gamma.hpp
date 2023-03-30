#pragma once
#include <glm/glm-all.hpp>

namespace renderme
{
	auto gamma(glm::vec3 color) -> glm::vec3;
	auto inv_gamma(glm::vec3 color) -> glm::vec3;
}