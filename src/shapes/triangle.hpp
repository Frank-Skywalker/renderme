#pragma once

#include <core/shape.hpp>
#include <core/geometry.hpp>
#include <core/primitive.hpp>

namespace renderme
{

	struct Triangle final: Shape
	{
		Triangle(Transform const* object_to_world, Transform const* world_to_object);
		auto gl_draw(Shader const& shader) const noexcept -> void;
		auto intersect() const noexcept ->bool;
		auto intersect_shadow() const noexcept ->bool;


		Mesh const* mesh;
		int face_index;
	};
}
