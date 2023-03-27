#include "path-tracer.hpp"

#include <core/shader.hpp>
#include <core/log.hpp>

#include <imgui/imgui.h>

#define RR_PATH_TRACER_MAX_ITERATION 1
#define RR_PATH_TRACER_EXPORT_IMG_AFTER_ITERATION 1

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
		auto new_hash = Hasher{}.hash_hash(std::size_t(&scene)).
			hash_hash(std::size_t(camera)).hash_hash(camera->hash()).
			hash_hash(std::size_t(sampler)).hash_hash(sampler->hash()).
			hash_hash(std::size_t(film)).hash_hash(film->hash()).value();

		if (last_hash != new_hash) {
			last_hash = new_hash;
			film->clear();
			iteration_counter = 0;
		}

		if (iteration_counter == RR_PATH_TRACER_MAX_ITERATION) {
			return;
		}

		++iteration_counter;
		log(Status::log, "Path Tracing Begins");
		for (auto x = 0; x < film->resolution().x; ++x) {
			for (auto y = 0; y < film->resolution().y; ++y) {
				auto sample = sampler->get_ndc_sample(glm::uvec2(x, y));
				auto ray = camera->generate_ray(sample);
				auto new_color = path_trace(std::move(ray), scene, 0);

				if (new_color != glm::vec3(0.f, 0.f, 0.f)) {
					auto& last_color = film->pixel_of(glm::uvec2(x, y));
					if (last_color != film->clear_color()) {
						new_color = (last_color * float(iteration_counter - 1) + new_color) / float(iteration_counter);
					}
					film->set_pixel(glm::uvec2(x, y), new_color);
				}
			}
		}

		if (iteration_counter % RR_PATH_TRACER_EXPORT_IMG_AFTER_ITERATION == 0) {
			film->export_to_file(Runtime_Path("/result/test.bmp"));
		}
	}

	auto Path_Tracer::imgui_config() ->void
	{
		if (ImGui::TreeNode("Shader")) {
			shader->imgui_config();
			ImGui::TreePop();
		}
	}


	auto Path_Tracer::path_trace(Ray ray, Scene const& scene, int depth) -> glm::vec3
	{
		Interaction interaction;
		scene.intersect(ray, &interaction);
		return interaction.color;
	}
}