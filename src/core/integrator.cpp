#include "integrator.hpp"

namespace renderme
{

	Integrator::Integrator(std::unique_ptr<Camera> camera, std::unique_ptr<Shader> shader)
		:camera{std::move(camera)}, shader{std::move(shader)}
	{
	}


	void Sample_Integrator::gl_draw(Scene const& scene)
	{
		scene.gl_draw(*shader);
	}

	void Sample_Integrator::render(Scene const& scene)
	{
		scene.intersect();
	}
}