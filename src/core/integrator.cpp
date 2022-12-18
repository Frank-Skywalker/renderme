#include "integrator.hpp"

namespace renderme
{

	Integrator::Integrator(std::unique_ptr<Camera> camera, std::unique_ptr<Shader> shader)
		:camera{std::move(camera)}, shader{std::move(shader)}
	{
	}


	auto Sample_Integrator::gl_draw(Scene const& scene) const noexcept -> void
	{
		scene.gl_draw(*shader);
	}

	auto Sample_Integrator::render(Scene const& scene) const noexcept -> void
	{
		scene.intersect();
	}
}