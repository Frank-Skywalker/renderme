#include "random.hpp"
#include <core/log.hpp>
#include <cstdlib>
namespace renderme
{
	Random_Sampler::Random_Sampler(glm::uvec2 _sample_space)
		:Sampler(_sample_space)
	{
		std::srand(std::time(nullptr));
	}


	auto Random_Sampler::get_ndc_sample(glm::uvec2 pos) -> glm::vec2
	{
		if (pos.x >= sample_space.x || pos.y >= sample_space.y) {
			log(Status::fatal, "Exists sample space");
		}

		auto random = glm::vec2(std::rand(), std::rand());
		return random / float(RAND_MAX) * 2.0f - 1.0f;
	}
}