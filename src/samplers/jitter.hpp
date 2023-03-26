#pragma once
#include <core/sampler.hpp>

namespace renderme
{
	struct Jitter_Sampler final : Sampler
	{
		Jitter_Sampler() = default;
		Jitter_Sampler(glm::uvec2 _sample_space);
		auto get_ndc_sample(glm::uvec2 pos) const noexcept -> glm::vec2 override;
	};

}