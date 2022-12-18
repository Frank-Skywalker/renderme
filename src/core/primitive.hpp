#pragma once
#include "util.hpp"
#include "shape.hpp"

#include<memory>
namespace renderme
{
	struct Primitive
	{
		virtual auto gl_draw(Shader const& shader) const noexcept -> void =0;
		virtual auto intersect() const noexcept ->bool = 0;
		virtual auto intersect_shadow() const noexcept ->bool= 0;
	};

	struct Shape_Primitive final: Primitive
	{
		Shape_Primitive(std::unique_ptr<Shape> shape);
		auto gl_draw(Shader const& shader) const noexcept -> void;
		auto intersect() const noexcept ->bool;
		auto intersect_shadow() const noexcept ->bool;

	private:
		std::unique_ptr<Shape> shape;
	};


	struct Aggregate: Primitive
	{
	};
}