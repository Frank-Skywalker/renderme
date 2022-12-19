#pragma once

#include "util.hpp"
#include "camera.hpp"
#include "scene.hpp"

namespace renderme
{

	struct Integrator : Only_Movable
	{
		Integrator(std::unique_ptr<Camera> camera, std::unique_ptr<Shader> shader);
		virtual auto gl_draw(Scene const& scene) const noexcept -> void = 0;
		virtual auto render(Scene const& scene) const noexcept -> void = 0;
		virtual auto imgui_config() ->void = 0;

		std::unique_ptr<Camera> camera;
		std::unique_ptr<Shader> shader;
	};


	struct Sample_Integrator: Integrator
	{
		Sample_Integrator(std::unique_ptr<Camera> camera, std::unique_ptr<Shader> shader);
		auto gl_draw(Scene const& scene) const noexcept -> void;
		auto render(Scene const& scene) const noexcept -> void;
		auto imgui_config() ->void;
	};

}