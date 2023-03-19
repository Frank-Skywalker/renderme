#pragma once

#include "transform.hpp"
#include "shader.hpp"
#include "bounds.hpp"

namespace renderme
{
	struct Shape
	{
		Shape(Transform const* object_to_world, Transform const* world_to_object);
		virtual auto gl_draw(Shader const& shader) const noexcept -> void = 0;
		virtual auto intersect() const noexcept ->bool = 0;
		virtual auto intersect_shadow() const noexcept ->bool = 0;
		virtual auto imgui_config() const noexcept ->void = 0;

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
