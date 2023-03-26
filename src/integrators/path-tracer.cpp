#include "path-tracer.hpp"

#include <core/shader.hpp>

#include <imgui/imgui.h>

namespace renderme
{
	Path_Tracer::Path_Tracer()
		:Integrator(std::make_unique<Shader>("src/shaders/normal.vert.glsl", "src/shaders/normal.frag.glsl"))
	{

	}

	auto Path_Tracer::gl_draw(Scene const& scene, Camera const* camera) -> void
	{
		//Be sure to enable shader before setting uniforms
		shader->use();
		camera->gl_draw(*shader);
		scene.gl_draw(*shader);

		// CAUTION!!!
		// Be sure to UNUSE shader after rendering
		// Or there may be bugs in afterwards rendering
		shader->unuse();
	}

	auto Path_Tracer::render(Scene const& scene, Camera const* camera, Sampler const* sampler, Film* film) -> void
	{
		for (auto x = 0; x < film->resolution().x; ++x) {
			for (auto y = 0; y < film->resolution().y; ++y) {
				//auto ndc_x = float(x) / float(film->resolution().x) * 2.0f - 1.0f;
				//auto ndc_y = float(y) / float(film->resolution().y) * 2.0f - 1.0f;
				auto sample = sampler->get_ndc_sample(glm::uvec2(x, y));
				auto ray = camera->generate_ray(sample);
				auto color = trace(std::move(ray), scene, 0);
				film->set_pixel(glm::uvec2(x, y), color);
			}
		}
	}

	auto Path_Tracer::imgui_config() ->void
	{
		if (ImGui::TreeNode("Shader")) {
			shader->imgui_config();
			ImGui::TreePop();
		}
	}


	auto Path_Tracer::trace(Ray ray, Scene const& scene, int depth) -> glm::vec3
	{
		Interaction interaction;
		scene.intersect(ray, &interaction);
		scene.intersect(ray, &interaction);
		return interaction.color;
	}
}