#pragma once
#include <core/sampler.hpp>

namespace renderme
{
	struct Jitter_Sampler final : Sampler
	{
		Jitter_Sampler(glm::uvec2 _sample_space);
		auto get_ndc_sample(glm::uvec2 pos) -> glm::vec2 override;
	};

}