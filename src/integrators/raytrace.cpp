#include "raytrace.hpp"

#include <core/shader.hpp>

#include <imgui/imgui.h>

namespace renderme
{
	Raytrace_Integrator::Raytrace_Integrator()
		:Integrator(std::make_unique<Shader>("src/shaders/normal.vert.glsl", "src/shaders/normal.frag.glsl"))
	{

	}

	auto Raytrace_Integrator::gl_draw(Camera const* camera, Scene const& scene) -> void
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

	auto Raytrace_Integrator::render(Camera const* camera, Scene const& scene, Film* film) -> void
	{
		for (auto x = 0; x < film->resolution().x; ++x) {
			for (auto y = 0; y < film->resolution().y; ++y) {
				auto ndc_x = float(x) / float(film->resolution().x) * 2.0f - 1.0f;
				auto ndc_y = float(y) / float(film->resolution().y) * 2.0f - 1.0f;
				auto ray = camera->generate_ray(glm::vec2(ndc_x, ndc_y));
				Interaction interaction;
				if (scene.intersect(ray, &interaction)) {
					film->set_pixel(glm::uvec2(x, y), interaction.color);
				}
			}
		}
	}

	auto Raytrace_Integrator::imgui_config() ->void
	{
		if (ImGui::TreeNode("Shader")) {
			shader->imgui_config();
			ImGui::TreePop();
		}
	}
}