#include"jitter.hpp"
#include <core/random.hpp>
#include <core/log.hpp>

namespace renderme
{
	Jitter_Sampler::Jitter_Sampler(glm::uvec2 _sample_space)
		:Sampler(_sample_space)
	{
		std::srand(std::time(nullptr));
	}

	auto Jitter_Sampler::get_ndc_sample(glm::uvec2 pos) const noexcept -> glm::vec2
	{
		if (pos.x >= sample_space.x || pos.y >= sample_space.y) {
			log(Status::fatal, "Exists sample space");
		}

		auto min_corner = glm::vec2(pos);
		auto offset = glm::vec2(random01(), random01());
		auto result_sample_space = min_corner + offset;
		return result_sample_space / glm::vec2(sample_space)  * 2.0f - 1.0f;
	}

	auto Jitter_Sampler::hash() const noexcept-> std::size_t
	{
		return std::size_t(3);
	}
}