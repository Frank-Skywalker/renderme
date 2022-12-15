#include "shape.hpp"

namespace renderme
{
	Shape::Shape(Transform const* object_to_world, Transform const* world_to_object)
		:object_to_world{object_to_world}, world_to_object{world_to_object}{}

}