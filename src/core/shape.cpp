#include "shape.hpp"

namespace renderme
{
	Shape::Shape(Transform const* object_to_world, Transform const* world_to_object)
		:_object_to_world{object_to_world}, _world_to_object{world_to_object}{}


	auto Shape::object_to_world() const noexcept-> Transform const*
	{
		return _object_to_world;
	}

	auto Shape::world_to_object() const noexcept-> Transform const*
	{
		return _world_to_object;
	}

	auto Shape::object_bounds() const noexcept-> Bounds3f const&
	{
		return _object_bounds;
	}

	auto Shape::world_bounds() const noexcept-> Bounds3f const&
	{
		return _world_bounds;
	}
}