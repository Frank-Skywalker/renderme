#pragma once
#include <glm/glm-all.hpp>

namespace renderme
{
	struct Sampler
	{
		Sampler() = default;
		Sampler(glm::uvec2 sample_space);

		virtual auto get_ndc_sample(glm::uvec2 pos) const noexcept -> glm::vec2 = 0;
		virtual auto hash() const noexcept-> std::size_t = 0;
		auto reset_sample_space(glm::uvec2 sample_space) -> void;

		glm::uvec2 sample_space{ 0,0 };
	};
}