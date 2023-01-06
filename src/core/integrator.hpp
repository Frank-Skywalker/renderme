#pragma once

#include "util.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "film.hpp"

namespace renderme
{

	struct Integrator : Only_Movable
	{
		Integrator(std::unique_ptr<Shader> shader);
		virtual auto gl_draw(Camera const* camera, Scene const& scene) const noexcept -> void = 0;
		virtual auto render(Camera const* camera, Scene const& scene, Film* film) const noexcept -> void = 0;
		virtual auto imgui_config() ->void = 0;

		std::unique_ptr<Shader> shader;
	};




}