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

	auto Shape_Primitive::intersect(Ray const& ray, Interaction* interaction) const noexcept ->bool
	{
		float t_hit;
		if (shape->intersect(ray, interaction, &t_hit)) {
			ray.update_tmax(t_hit);
			interaction->material = material;
			interaction->primitive = this;
			return true;
		}
		return false;
	}
	auto Shape_Primitive::intersect_shadow(Ray const& ray) const noexcept ->bool
	{
		return shape->intersect_shadow(ray);
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
