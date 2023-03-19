#include"primitive.hpp"

namespace renderme
{
	Shape_Primitive::Shape_Primitive(
		Shape const* shape,
		Material const* material
		)
		:shape{shape},
		material{material}
	{
	}

	auto Shape_Primitive::gl_draw(Shader const& shader) const noexcept -> void
	{
		material->gl_draw(shader);
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

	auto Shape_Primitive::imgui_config() const noexcept ->void
	{
		shape->imgui_config();
	}

	auto Shape_Primitive::world_bounds() const noexcept -> Bounds3f const&
	{
		return shape->world_bounds();
	}



}
