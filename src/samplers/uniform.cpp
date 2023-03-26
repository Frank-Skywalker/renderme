#include "uniform.hpp"
#include <core/log.hpp>

namespace renderme
{
	Uniform_Sampler::Uniform_Sampler(glm::uvec2 _sample_space)
		:Sampler(_sample_space)
	{}


	auto Uniform_Sampler::get_ndc_sample(glm::uvec2 pos) const noexcept -> glm::vec2
	{
		if (pos.x >= sample_space.x || pos.y >= sample_space.y) {
			log(Status::fatal, "Exists sample space");
		}

		auto result_sample_space = glm::vec2(pos) + glm::vec2(0.5f, 0.5f);
		return result_sample_space / glm::vec2(sample_space) * 2.0f - 1.0f;
	}

	auto Uniform_Sampler::hash() const noexcept-> std::size_t
	{
		return std::size_t(1);
	}


}