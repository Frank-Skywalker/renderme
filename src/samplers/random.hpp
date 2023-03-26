#pragma once
#include <core/sampler.hpp>

namespace renderme
{
	struct Random_Sampler final : Sampler
	{
		Random_Sampler() = default;
		Random_Sampler(glm::uvec2 sample_space);
		auto get_ndc_sample(glm::uvec2 pos) const noexcept ->glm::vec2 override;
	};

}