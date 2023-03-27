#pragma once
#include "material.hpp"
#include "interaction.hpp"
#include "shape.hpp"

namespace renderme
{
	struct Scene;
	struct Light
	{
		virtual auto light_up(Interaction const& interaction, Scene const& scene) -> glm::vec3 = 0;
	};

	struct Area_Light final: Light {
		Area_Light(Material const* material, glm::vec3 radiance, Shape const* shape);

		auto light_up(Interaction const& interaction, Scene const& scene) -> glm::vec3 override;

		Shape const* shape;
		Material const* material;
		glm::vec3 radiance;

	};
}