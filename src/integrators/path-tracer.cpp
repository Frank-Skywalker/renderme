#include "path-tracer.hpp"

#include <core/shader.hpp>
#include <core/log.hpp>

#include <materials/phong.hpp>

#include <imgui/imgui.h>

#define RR_PATH_TRACER_MAX_ITERATION 10
#define RR_PATH_TRACER_EXPORT_IMG_AFTER_ITERATION 2
#define RR_PATH_TRACER_MAX_DEPTH 5

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
				auto new_color = trace(std::move(ray), scene, 0);

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
			auto path = "/result/" + scene.name + ".bmp";
			film->export_to_file(path);
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
		// Ray has no intersection with the scene
		if (!scene.intersect(ray, &interaction)) {
			return interaction.color;
		}

		// I don't wanna do this...
		// Can't think of a better architecture for now
		auto material = dynamic_cast<Phong_Material const*>(interaction.material);

		glm::vec3 result = material->emition;
		// Maximum depth reached
		if (depth > RR_PATH_TRACER_MAX_DEPTH) {
			return result;
		}

		ray = mont_carlo_sample_new_ray(ray, interaction);

		// Compute indirect light component
		auto indirect_component = trace(ray, scene, depth + 1);
		switch (ray.type) {
		case Ray_Type::diffuse:
			indirect_component *= material->diffuse;
			break;
		case Ray_Type::specular:
			indirect_component *= material->specular;
			break;
		case Ray_Type::transparent:
			indirect_component *= material->transparent;
			break;
		default:
			log(Status::fatal, "Invalid ray type");
		}
		result += indirect_component;

		// Compute direct light component
		auto direct_component = compute_direct_light(ray, interaction, scene);
		result += direct_component;

		return result;
	}

	auto fresnel(glm::vec3 const& ray_dir, glm::vec3 const& normal, float ri_from, float ri_to) -> float
	{
		// Calculate frenel according to Schlick method
		auto cos_von = glm::dot(ray_dir, normal);
		auto f0 = (ri_from - ri_to) / (ri_from + ri_to);
		f0 *= f0;
		auto fc = std::powf(1 - std::fabs(cos_von), 5.0f);
		return f0 + (1.f - f0) * fc;
	}

	auto reflect_direction(glm::vec3 const& ray_dir, glm::vec3 const& normal) -> glm::vec3
	{
		auto dir = ray_dir - 2 * glm::dot(ray_dir, normal) * normal;
		return glm::normalize(dir);
	}

	auto refract_direction(glm::vec3 const& ray_dir, glm::vec3 const& normal, float ri_from, float ri_to) -> glm::vec3
	{
		auto inv_ray= -ray_dir;
		float ri_ratio = ri_from / ri_to;
		auto cos_theta = glm::dot(normal, inv_ray);
		auto dir = (ri_ratio * cos_theta - std::sqrtf(1 - std::powf(ri_ratio, 2.f) * (1 - std::powf(cos_theta, 2.f)))) * normal - ri_ratio * inv_ray;
		return glm::normalize(dir);
	}

	auto Path_Tracer::mont_carlo_sample_new_ray(Ray const& ray, Interaction const& interaction) -> Ray
	{
		auto material = dynamic_cast<Phong_Material const*>(interaction.material);

		// Try generate with refraction ray
		if (material->refraction_index > 1.0f) {
			auto cos_von = glm::dot( -ray.direction, interaction.normal);
			// Refract index of both sides of the surface
			// Init to refract index of the air(1.0f)
			float ri_from = 1.0f;
			float ri_to = 1.0f;

			// Ray direction from outside to inside of the surface
			if (cos_von > 0) {
				ri_to = material->refraction_index;
			}
			// Ray direction from inside to outside of the surface
			else {
				ri_from = material->refraction_index;
			}

			auto fres = fresnel(ray.direction, interaction.normal, ri_from, ri_to);
			auto random = russian_roulette();
			// Create refrac ray
			if (random > fres) {
				auto refract_dir = refract_direction(ray.direction, interaction.normal, ri_from, ri_to);
				return Ray(Ray_Type::refract, interaction.position, refract_dir, RR_EPSILON);
			}
		}





		return Ray(ray);
	}


	auto Path_Tracer::compute_direct_light(Ray const& ray, Interaction const& interaction, Scene const& camera) -> glm::vec3
	{
		return glm::vec3();
	}


	auto Path_Tracer::russian_roulette() -> float
	{
		return float(std::rand()) / float(RAND_MAX);
	}

}