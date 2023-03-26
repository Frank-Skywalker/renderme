#pragma once
#include <core/integrator.hpp>

namespace renderme
{
	struct Path_Tracer final : Integrator
	{
		Path_Tracer();

		auto gl_draw(Scene const& scene, Camera const* camera) -> void override;
		auto render(Scene const& scene, Camera const* camera, Sampler const* sampler, Film* film) -> void override;
		auto imgui_config() -> void override;

	private:
		auto trace(Ray ray, Scene const& scene, int depth) -> glm::vec3;

		std::size_t last_hash;
		int iteration_counter{ 0 };
	};

}