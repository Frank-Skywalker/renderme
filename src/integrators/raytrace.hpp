#pragma once
#include <core/integrator.hpp>

namespace renderme
{
	struct Raytrace_Integrator final : Integrator
	{


		Raytrace_Integrator();

		auto gl_draw(Camera const* camera, Scene const& scene) -> void;
		auto render(Camera const* camera, Scene const& scene, Film* film) -> void;
		auto imgui_config() -> void;

	};

}