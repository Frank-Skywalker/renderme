#pragma once
#include "util.hpp"
#include "shape.hpp"
#include "material.hpp"

#include<memory>
namespace renderme
{
	struct Primitive
	{
		virtual auto gl_draw(Shader const& shader) const noexcept -> void =0;
		virtual auto intersect(Ray const& ray, Interaction* interaction) const noexcept ->bool = 0;
		virtual auto intersect_shadow(Ray const& ray) const noexcept ->bool= 0;
		virtual auto imgui_config() const noexcept ->void = 0;

		virtual auto world_bounds() const noexcept -> Bounds3f const& = 0;
	};

	struct Shape_Primitive final: Primitive
	{
		Shape_Primitive(Shape const* shape, Material const* material);
		auto gl_draw(Shader const& shader) const noexcept -> void final;
		auto intersect(Ray const& ray, Interaction* interaction) const noexcept ->bool final;
		auto intersect_shadow(Ray const& ray) const noexcept ->bool final;
		auto imgui_config() const noexcept ->void final;

		auto world_bounds() const noexcept -> Bounds3f const& final;

		Shape const* shape;
		Material const* material;
	};


	struct Aggregate: Primitive
	{
	};
}
