#include "light.hpp"
#include "log.hpp"
#include "scene.hpp"
#include <numbers>
#define RR_AREA_LIGHT_SAMPLE_NUM 3

namespace renderme
{
	Area_Light::Area_Light(Material const* _material, glm::vec3 _radiance, Shape const* _shape)
		:material{_material}, radiance{_radiance}, shape{_shape}
	{
		if (radiance == glm::vec3(0.f, 0.f, 0.f)) {
			log(Status::fatal, "Create black area light");
		}
	}

	auto Area_Light::light_up(Interaction const& interaction, Scene const& scene) -> glm::vec3
	{
		auto result = glm::vec3(0.f, 0.f, 0.f);

		auto& material = interaction.material;
		for (auto i = 0u; i < RR_AREA_LIGHT_SAMPLE_NUM; ++i) {
			// Randomly create sample uvw
			auto uvw = glm::vec3(std::rand() / RAND_MAX, std::rand() / RAND_MAX, std::rand() / RAND_MAX);
			// Get sampled light origin
			auto light_origin = shape->position_of(uvw);
			auto shadow_dir = light_origin - interaction.position;
			auto distance = glm::length(shadow_dir);
			shadow_dir = glm::normalize(shadow_dir);
			// Create shadow ray
			auto shadow_ray = Ray(interaction.position, shadow_dir, RR_EPSILON, distance - RR_EPSILON);

			// Intersect shadow ray with the scene
			if (!scene.intersect_shadow(shadow_ray)) {
				auto light_normal = shape->normal_of(uvw);
				// At intersection point
				float cos_theta_in = std::fmax(glm::dot(interaction.normal, shadow_dir), 0.0f);
				// At light source
				float cos_theta_out = std::fmax(glm::dot(-shadow_dir, light_normal), 0.0f);
				float geometry_factor = cos_theta_in * cos_theta_out / (distance * distance);

				auto intensity = geometry_factor * shape->surface_area() * radiance / float(RR_AREA_LIGHT_SAMPLE_NUM);

				float cos_theta = glm::dot(shadow_dir, interaction.normal);
				if (cos_theta > 0.f) {
					// Diffuse color at intersection point
					result += cos_theta * material->diffuse * intensity / float(std::numbers::pi);
					// Specular color at intersection point
					result += std::powf(cos_theta, material->specular_exponent) * material->specular * intensity 
						* (material->specular_exponent + 1) / (2.f * float(std::numbers::pi));
				}
			}

		}

		return result;
	}
}