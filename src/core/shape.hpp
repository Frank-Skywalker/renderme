#pragma once

#include "transform.hpp"
#include "shader.hpp"
#include "bounds.hpp"
#include "ray.hpp"
#include "interaction.hpp"

namespace renderme
{
	struct Shape
	{
		Shape(Transform const* object_to_world, Transform const* world_to_object);
		virtual auto gl_draw(Shader const& shader) const noexcept -> void = 0;
		virtual auto intersect(Ray const& ray, Interaction* interaction, float* t) const noexcept ->bool = 0;
		virtual auto intersect_shadow(Ray const& ray) const noexcept ->bool = 0;
		virtual auto imgui_config() const noexcept ->void = 0;

		virtual auto surface_area() const noexcept -> float = 0;
		virtual auto uvw_of(glm::vec3 point) const noexcept -> glm::vec3 = 0;
		virtual auto position_of(glm::vec3 uvw) const noexcept -> glm::vec3 = 0;
		virtual auto normal_of(glm::vec3 uvw) const noexcept -> glm::vec3 = 0;
		virtual auto texture_coordinate_of(glm::vec3 uvw) const noexcept -> glm::vec2 =0;

		auto object_to_world() const noexcept -> Transform const*;
		auto world_to_object() const noexcept -> Transform const*;

		auto object_bounds() const noexcept -> Bounds3f const&;
		auto world_bounds() const noexcept -> Bounds3f const&;


	protected:
		Transform const* _object_to_world;
		Transform const* _world_to_object;

		Bounds3f _object_bounds;
		Bounds3f _world_bounds;
	};
}
