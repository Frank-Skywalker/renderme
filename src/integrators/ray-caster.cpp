#include "ray-caster.hpp"

#include <core/shader.hpp>
#include <core/log.hpp>

#include <imgui/imgui.h>


namespace renderme
{
	Ray_Caster::Ray_Caster()
		:Integrator(std::make_unique<Shader>("src/shaders/normal.vert.glsl", "src/shaders/normal.frag.glsl"))
	{

	}

	auto Ray_Caster::gl_draw(Scene const& scene, Camera const* camera) -> void
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

	auto Ray_Caster::render(Scene const& scene, Camera const* camera, Sampler const* sampler, Film* film) -> void
	{
		auto new_hash = Hasher{}.hash_hash(std::size_t(&scene)).
			hash_hash(std::size_t(camera)).hash_hash(camera->hash()).
			hash_hash(std::size_t(sampler)).hash_hash(sampler->hash()).
			hash_hash(std::size_t(film)).hash_hash(film->hash()).value();

		if (last_hash == new_hash) {
			return;
		}

		last_hash = new_hash;
		film->clear();

#pragma omp parallel for schedule(dynamic, 1)
		for (auto x = 0; x < film->resolution().x; ++x) {
			for (auto y = 0; y < film->resolution().y; ++y) {
				auto sample = sampler->get_ndc_sample(glm::uvec2(x, y));
				auto ray = camera->generate_ray(sample);
				auto color = cast(std::move(ray), scene);
				if (color != glm::vec3(0.f, 0.f, 0.f)) {
					film->set_pixel(glm::uvec2(x, y), color);
				}
			}
		}

	}

	auto Ray_Caster::imgui_config() ->void
	{
		if (ImGui::TreeNode("Shader")) {
			shader->imgui_config();
			ImGui::TreePop();
		}
	}


	auto Ray_Caster::cast(Ray ray, Scene const& scene) -> glm::vec3
	{
		Interaction interaction;
		// Ray has no intersection with the scene
		if (!scene.intersect(ray, &interaction)) {
			return glm::vec3(0.f, 0.f, 0.f);
		}

		return interaction.material->diffuse(interaction.uv);
	}

}