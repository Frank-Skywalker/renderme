#pragma once
#include <core/integrator.hpp>

namespace renderme
{
	struct Ray_Caster final : Integrator
	{
		enum struct Ray_Type
		{
			diffuse,
			specular,
			refract,
		};

		Ray_Caster();

		auto gl_draw(Scene const& scene, Camera const* camera) -> void override;
		auto render(Scene const& scene, Camera const* camera, Sampler const* sampler, Film* film) -> void override;
		auto imgui_config() -> void override;

	private:
		auto cast(Ray ray, Scene const& scene) -> glm::vec3;

		std::size_t last_hash;
	};

}