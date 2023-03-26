#pragma once
#include <glm/glm-all.hpp>

namespace renderme
{
	struct Sampler
	{
		Sampler(glm::uvec2 sample_space);

		virtual auto get_ndc_sample(glm::uvec2 pos) -> glm::vec2 = 0;
		auto reset_sample_space(glm::uvec2 sample_space) -> void;

		glm::uvec2 sample_space;
	};
}