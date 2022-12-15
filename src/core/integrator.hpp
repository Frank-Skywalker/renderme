#pragma once

#include "util.hpp"
#include "camera.hpp"
#include "scene.hpp"

namespace renderme
{

	struct Integrator : Only_Movable
	{
		Integrator(std::unique_ptr<Camera> camera, std::unique_ptr<Shader> shader);
		virtual void gl_draw(Scene const& scene) = 0;
		virtual void render(Scene const& scene) = 0;

		std::unique_ptr<Camera> camera;
		std::unique_ptr<Shader> shader;
	};


	struct Sample_Integrator: Integrator
	{
		void gl_draw(Scene const& scene);
		void render(Scene const& scene);
	};

}