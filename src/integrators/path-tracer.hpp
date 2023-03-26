#pragma once
#include <core/integrator.hpp>

namespace renderme
{
	struct Path_Tracer final : Integrator
	{
		Path_Tracer();

		auto gl_draw(Camera const* camera, Scene const& scene) -> void override;
		auto render(Camera const* camera, Scene const& scene, Film* film) -> void override;
		auto imgui_config() -> void override;

	private:
		auto trace(Ray ray, Scene const& scene, int depth) -> glm::vec3;

	};

}