#pragma once

#include "transform.hpp"
#include "shader.hpp"

namespace renderme
{
	struct Shape
	{
		Shape(Transform const* object_to_world, Transform const* world_to_object);
		virtual auto gl_draw(Shader const& shader) const noexcept -> void = 0;
		virtual auto intersect() const noexcept ->bool = 0;
		virtual auto intersect_shadow() const noexcept ->bool = 0;
		virtual auto imgui_config() const noexcept ->void = 0;

		Transform const* object_to_world;
		Transform const* world_to_object;
	};
}
