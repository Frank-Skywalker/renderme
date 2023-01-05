#pragma once
#include<core/vector.hpp>
#include<core/util.hpp>
#include<memory>

namespace renderme
{
	struct Pixel final
	{
		Float r;
		Float g;
		Float b;
	};

	struct Film final: Non_Transferable
	{
		Film()=default;
		Film(Point2i resolution);

		auto reset_resolution(Point2i reslotion)->void;
		auto set_pixel(unsigned int x, unsigned int y, Vector3f p) ->void;
		auto resolution() const noexcept ->Point2i const&;

		auto gl_display() const noexcept ->void;
		
	private:
		std::unique_ptr<Pixel[]> pixels;
		Point2i _resolution;
	};
}