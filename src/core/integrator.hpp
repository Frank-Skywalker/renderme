#pragma once

#include "util.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "film.hpp"
#include "sampler.hpp"

namespace renderme
{

	struct Integrator : Only_Movable
	{
		Integrator(std::unique_ptr<Shader> shader);
		virtual auto gl_draw(Scene const& scene, Camera const* camera) -> void = 0;
		virtual auto render(Scene const& scene, Camera const* camera, Sampler const* sampler, Film* film) -> void = 0;
		virtual auto imgui_config() ->void = 0;

		std::unique_ptr<Shader> shader;
	};




}