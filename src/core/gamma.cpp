#include "gamma.hpp"
#define RR_GAMMA 2.2f
namespace renderme
{
	auto gamma(glm::vec3 color) -> glm::vec3
	{
		return glm::vec3(std::powf(color.x, RR_GAMMA), std::powf(color.y, RR_GAMMA), std::powf(color.z, RR_GAMMA));
	}

	auto inv_gamma(glm::vec3 color) -> glm::vec3
	{
		return glm::vec3(std::powf(color.x, 1.f / RR_GAMMA), std::powf(color.y, 1.f / RR_GAMMA), std::powf(color.z, 1.f / RR_GAMMA));
	}
}