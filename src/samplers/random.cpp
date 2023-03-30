#include "random.hpp"
#include <core/random.hpp>
#include <core/log.hpp>
namespace renderme
{
	Random_Sampler::Random_Sampler(glm::uvec2 _sample_space)
		:Sampler(_sample_space)
	{
		std::srand(std::time(nullptr));
	}


	auto Random_Sampler::get_ndc_sample(glm::uvec2 pos) const noexcept -> glm::vec2
	{
		if (pos.x >= sample_space.x || pos.y >= sample_space.y) {
			log(Status::fatal, "Exists sample space");
		}

		auto random = glm::vec2(random01(), random01());
		return random * 2.0f - 1.0f;
	}

	auto Random_Sampler::hash() const noexcept-> std::size_t
	{
		return std::size_t(2);
	}

}