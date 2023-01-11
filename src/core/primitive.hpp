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
		virtual auto intersect() const noexcept ->bool = 0;
		virtual auto intersect_shadow() const noexcept ->bool= 0;
		virtual auto imgui_config() const noexcept ->void = 0;
	};

	struct Shape_Primitive final: Primitive
	{
		Shape_Primitive(std::unique_ptr<Shape> shape, std::shared_ptr<Material> material);
		auto gl_draw(Shader const& shader) const noexcept -> void;
		auto intersect() const noexcept ->bool;
		auto intersect_shadow() const noexcept ->bool;
		auto imgui_config() const noexcept ->void;

		std::unique_ptr<Shape> shape;
		std::shared_ptr<Material> material;
	};


	struct Aggregate: Primitive
	{
	};
}