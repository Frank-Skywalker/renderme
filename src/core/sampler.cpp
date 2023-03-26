#include "sampler.hpp"

namespace renderme
{
	Sampler::Sampler(glm::uvec2 _sample_space)
		:sample_space{ _sample_space }
	{}

	auto Sampler::reset_sample_space(glm::uvec2 _sample_space) -> void
	{
		sample_space = _sample_space;
	}

}