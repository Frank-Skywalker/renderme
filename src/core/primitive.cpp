#include"primitive.hpp"

namespace renderme
{
	Shape_Primitive::Shape_Primitive(std::unique_ptr<Shape> shape)
		:shape{std::move(shape)}
	{
	}

	auto Shape_Primitive::gl_draw(Shader const& shader) const noexcept -> void
	{
		shape->gl_draw(shader);
	}

	auto Shape_Primitive::intersect() const noexcept ->bool
	{
		return shape->intersect();
	}

	auto Shape_Primitive::intersect_shadow() const noexcept ->bool
	{
		return shape->intersect_shadow();
	}

}