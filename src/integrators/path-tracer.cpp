#include "path-tracer.hpp"

#include <core/shader.hpp>
#include <core/random.hpp>
#include <core/gamma.hpp>
#include <core/log.hpp>

#include <imgui/imgui.h>

#include <numbers>
#include <thread>
#include <chrono>

#define RR_PATH_TRACER_MAX_ITERATION 1000
#define RR_PATH_TRACER_EXPORT_IMG_AFTER_ITERATION 10
#define RR_PATH_TRACER_MAX_DEPTH 5

#define RR_PATH_TRACING_THREAD_COUNT_X 4
#define RR_PATH_TRACING_THREAD_COUNT_Y 4

#define RR_PATH_TRACING_COMPUTE_DIRECT_LIGHT

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
			total_time = 0.f;
		}

		if (iteration_counter == RR_PATH_TRACER_MAX_ITERATION) {
			return;
		}

		++iteration_counter;
		auto t_begin = std::chrono::high_resolution_clock().now();

		std::vector<std::thread> threads;
		int pixels_per_tile_x = std::ceil(float(film->resolution().x) / float(RR_PATH_TRACING_THREAD_COUNT_X));
		int pixels_per_tile_y = std::ceil(float(film->resolution().y) / float(RR_PATH_TRACING_THREAD_COUNT_Y));

		auto render_tile = [&](int x_tile, int y_tile)->void {
			auto x_begin = x_tile * pixels_per_tile_x;
			auto y_begin = y_tile * pixels_per_tile_y;

			for (auto x = x_begin; x < x_begin + pixels_per_tile_x && x < film->resolution().x; ++x) {
				for (auto y = y_begin; y < y_begin + pixels_per_tile_y && y < film->resolution().y; ++y) {
					auto sample = sampler->get_ndc_sample(glm::uvec2(x, y));
					auto ray = camera->generate_ray(sample);
					auto new_color = trace(std::move(ray), scene, 0);

					// Do gamma transform
					new_color = inv_gamma(new_color);
					auto& last_color = film->pixel_of(glm::uvec2(x, y));
					if (last_color != film->clear_color()) {
						new_color = (last_color * float(iteration_counter - 1) + new_color) / float(iteration_counter);
					}
					film->set_pixel(glm::uvec2(x, y), new_color);

				}
			}
		};

		for (auto x_tile = 0u; x_tile < RR_PATH_TRACING_THREAD_COUNT_X; ++x_tile) {
			for (auto y_tile = 0u; y_tile < RR_PATH_TRACING_THREAD_COUNT_Y; ++y_tile) {
				threads.push_back(std::thread(render_tile, x_tile, y_tile));
			}
		}

		for (auto& thread : threads) {
			thread.join();
		}

		auto t_end = std::chrono::high_resolution_clock().now();
		auto t_duration = std::chrono::duration_cast<std::chrono::duration<float>>(t_end - t_begin).count();
		total_time += t_duration;
		std::string msg = "Path Tracing Iteration " + std::to_string(iteration_counter) + "\n"
			+ "  Iteration time " + std::to_string(t_duration) + "s\n"
			+ "  Total time " + std::to_string(total_time / 60.f) + "min\n";
		log(Status::log, msg);

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


	auto fresnel(float cos, float ri_from, float ri_to) -> float
	{
		// Calculate frenel according to Schlick method
		auto f0 = (ri_from - ri_to) / (ri_from + ri_to);
		f0 *= f0;
		auto fc = std::powf(1.f - std::fabs(cos), 5.0f);
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


	auto brdf_importance_sample_diffuse(glm::vec3 const& main_dir) -> glm::vec3
	{
		// Sample over sphere coordinates
		float phi = random01() * 2.f * std::numbers::pi;
		float theta = std::asinf(std::sqrtf(random01()));

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
		float phi = random01() * 2.f * std::numbers::pi;
		float theta = std::acosf(std::powf(random01(), 1.f / (specular_exponent + 1.f)));

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

	auto mont_carlo_sample_new_ray(Ray const& ray, Interaction const& interaction, Path_Tracer::Ray_Type* out_type, float* out_russian_roulette) -> Ray
	{
		auto material = interaction.material;
		auto uv = interaction.uv;

		// Try generate with refraction ray
		if (material->refraction_index(uv) != 1.0f) {
			auto cos_von = glm::dot(ray.direction, interaction.normal);
			glm::vec3 refract_normal;
			// Refract index of both sides of the surface
			// Init to refract index of the air(1.0f)
			float ri_from = 1.0f;
			float ri_to = 1.0f;

			// Ray direction from outside to inside of the surface
			if (cos_von < 0) {
				ri_to = material->refraction_index(uv);
				refract_normal = interaction.normal;
			}
			// Ray direction from inside to outside of the surface
			else {
				ri_from = material->refraction_index(uv);
				refract_normal = -interaction.normal;
			}

			auto fres = fresnel(cos_von, ri_from, ri_to);
			auto rr = random01();
			if (fres < rr) {
				// Create refract ray
				auto refract_dir = refract_direction(ray.direction, refract_normal, ri_from, ri_to);
				*out_type = Path_Tracer::Ray_Type::refract;
				*out_russian_roulette = rr;
				return Ray(interaction.position, refract_dir, RR_EPSILON);
			}
			else {
				// Create reflect ray
				auto main_dir = reflect_direction(ray.direction, refract_normal);
				// Set Ray_Type as refract here to make sure it has color(ks may be 0,0,0)
				*out_type = Path_Tracer::Ray_Type::refract;
				*out_russian_roulette = rr;
				return Ray(interaction.position, main_dir, RR_EPSILON);
			}
		}

		auto diffuse_strength = glm::length(material->diffuse(uv));
		auto specular_strength = glm::length(material->specular(uv));
		auto percentage = diffuse_strength / (diffuse_strength + specular_strength);
		auto rr = random01();
		if (percentage < rr) {
			auto main_dir = reflect_direction(ray.direction, interaction.normal);
			auto sample_dir = brdf_importance_sample_specular(main_dir, material->specular_exponent(uv));
			*out_type = Path_Tracer::Ray_Type::specular;
			*out_russian_roulette = rr;
			return Ray(interaction.position, sample_dir, RR_EPSILON);
		}
		else {
			auto sample_dir = brdf_importance_sample_diffuse(interaction.normal);
			*out_type = Path_Tracer::Ray_Type::diffuse;
			*out_russian_roulette = rr;
			return Ray(interaction.position, sample_dir, RR_EPSILON);
		}
	}


	auto compute_direct_light(Interaction const& interaction, Scene const& scene) -> glm::vec3
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

		auto material = interaction.material;
		auto uv = interaction.uv;

		// Add emisive color and ambient color
		glm::vec3 result = material->emition(uv);

		// Maximum depth reached
		if (depth >= RR_PATH_TRACER_MAX_DEPTH) {
			return result;
		}

		Ray_Type type;
		float russian_roulette;
		ray = mont_carlo_sample_new_ray(ray, interaction, &type, &russian_roulette);

		// Compute indirect light component
		auto indirect_component = trace(ray, scene, depth + 1);

#ifdef RR_PATH_TRACING_COMPUTE_DIRECT_LIGHT
		// Do russian roulette
		indirect_component /= russian_roulette;
#endif

		switch (type) {
		case Ray_Type::diffuse:
			indirect_component *= material->diffuse(uv);
			break;
		case Ray_Type::specular:
			indirect_component *= material->specular(uv);
			break;
		case Ray_Type::refract:
			indirect_component *= material->transparent(uv);
			break;
		default:
			log(Status::fatal, "Invalid ray type");
		}
		//result += indirect_component + material->ambient(uv);
		result += indirect_component;

#ifdef RR_PATH_TRACING_COMPUTE_DIRECT_LIGHT
		// Compute direct light component
		auto direct_component = compute_direct_light(interaction, scene);
		result += direct_component;
#endif

		return result;
	}

}