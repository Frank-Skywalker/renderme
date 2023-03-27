#include "path-tracer.hpp"

#include <core/shader.hpp>
#include <core/log.hpp>

#include <imgui/imgui.h>

#include <numbers>

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
		auto inv_ray = -ray_dir;
		float ri_ratio = ri_from / ri_to;
		auto cos_theta = glm::dot(normal, inv_ray);
		auto dir = (ri_ratio * cos_theta - std::sqrtf(1 - std::powf(ri_ratio, 2.f) * (1 - std::powf(cos_theta, 2.f)))) * normal - ri_ratio * inv_ray;
		return glm::normalize(dir);
	}

	auto russian_roulette() -> float
	{
		return float(std::rand()) / float(RAND_MAX);
	}

	auto brdf_importance_sample_diffuse(glm::vec3 const& main_dir) -> glm::vec3
	{
		// Sample over sphere coordinates
		float phi = russian_roulette() * 2.f * std::numbers::pi;
		float theta = std::asinf(std::sqrtf(russian_roulette()));

		glm::vec3 sample(std::sinf(theta) * std::cosf(phi), std::cosf(theta), std::sinf(theta) * std::sinf(phi));
		glm::vec3 front;
		if (std::fabs(main_dir.x) > std::fabs(main_dir.y)) {
			front = glm::vec3(main_dir.z, 0, -main_dir.x);
		}
		else {
			front = glm::vec3(0, -main_dir.z, main_dir.y);
		}
		front = glm::normalize(front);
		glm::vec3 right = glm::cross(main_dir, front);

		return glm::normalize(sample.x * right + sample.y * main_dir + sample.z * front);
	}

	auto brdf_importance_sample_specular(glm::vec3 const& main_dir, float specular_exponent) -> glm::vec3
	{
		// Sample over sphere coordinates
		float phi = russian_roulette() * 2.f * std::numbers::pi;
		float theta = std::acosf(std::powf(russian_roulette(), 1.f / (specular_exponent + 1.f)));

		glm::vec3 sample(std::sinf(theta) * std::cosf(phi), std::cosf(theta), std::sinf(theta) * std::sinf(phi));
		glm::vec3 front;
		if (std::fabs(main_dir.x) > std::fabs(main_dir.y)) {
			front = glm::vec3(main_dir.z, 0, -main_dir.x);
		}
		else {
			front = glm::vec3(0, -main_dir.z, main_dir.y);
		}
		front = glm::normalize(front);
		glm::vec3 right = glm::cross(main_dir, front);

		return glm::normalize(sample.x * right + sample.y * main_dir + sample.z * front);
	}

	auto mont_carlo_sample_new_ray(Ray const& ray, Interaction const& interaction, Path_Tracer::Ray_Type* out_type) -> Ray
	{
		auto material = interaction.material;

		// Try generate with refraction ray
		if (material->refraction_index > 1.0f) {
			auto cos_von = glm::dot(-ray.direction, interaction.normal);
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
			auto percentage = russian_roulette();
			if (fres < percentage) {
				// Create refrac ray
				auto refract_dir = refract_direction(ray.direction, interaction.normal, ri_from, ri_to);
				*out_type = Path_Tracer::Ray_Type::refract;
				return Ray(interaction.position, refract_dir, RR_EPSILON);
			}
		}

		auto kd_ks_ratio = glm::length(material->diffuse) / glm::length(material->specular);
		auto percentage = russian_roulette();
		if (kd_ks_ratio < percentage) {
			auto main_dir = reflect_direction(ray.direction, interaction.normal);
			auto sample_dir = brdf_importance_sample_specular(main_dir, material->specular_exponent);
			*out_type = Path_Tracer::Ray_Type::specular;
			return Ray(interaction.position, sample_dir, RR_EPSILON);
		}
		else {
			auto sample_dir = brdf_importance_sample_diffuse(interaction.normal);
			*out_type = Path_Tracer::Ray_Type::diffuse;
			return Ray(interaction.position, sample_dir, RR_EPSILON);
		}
	}


	auto compute_direct_light(Ray const& ray, Interaction const& interaction, Scene const& scene) -> glm::vec3
	{
		glm::vec3 direct_light(0.f, 0.f, 0.f);
		for (auto const& light : scene.lights) {
			direct_light += light->light_up(interaction, scene);
		}
		return direct_light;
	}

	auto Path_Tracer::trace(Ray ray, Scene const& scene, int depth) -> glm::vec3
	{
		Interaction interaction;
		// Ray has no intersection with the scene
		if (!scene.intersect(ray, &interaction)) {
			return glm::vec3(0.f, 0.f, 0.f);
		}

		// I don't wanna do this...
		// Can't think of a better architecture for now
		//auto material = dynamic_cast<Phong_Material const*>(interaction.material);
		auto material = interaction.material;

		glm::vec3 result = material->emition;
		// Maximum depth reached
		if (depth > RR_PATH_TRACER_MAX_DEPTH) {
			return result;
		}

		Ray_Type type;
		ray = mont_carlo_sample_new_ray(ray, interaction, &type);

		// Compute indirect light component
		auto indirect_component = trace(ray, scene, depth + 1);
		switch (type) {
		case Ray_Type::diffuse:
			indirect_component *= material->diffuse;
			break;
		case Ray_Type::specular:
			indirect_component *= material->specular;
			break;
		case Ray_Type::refract:
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

}