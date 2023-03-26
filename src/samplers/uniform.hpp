#pragma once
#include <core/sampler.hpp>

namespace renderme
{
	struct Uniform_Sampler final : Sampler
	{
		Uniform_Sampler() = default;
		Uniform_Sampler(glm::uvec2 sample_space);
		auto get_ndc_sample(glm::uvec2 pos) const noexcept -> glm::vec2 override;
		auto hash() const noexcept-> std::size_t override;
	};


}