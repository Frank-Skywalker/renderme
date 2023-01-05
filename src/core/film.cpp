#pragma once

#include"film.hpp"
#include"log.hpp"

namespace renderme
{

	Film::Film(Point2i res)
		:_resolution{res}
	{
		pixels = std::make_unique<Pixel[]>(_resolution.x * _resolution.y);
	}


	auto Film::resolution() const noexcept ->Point2i const&
	{
		return _resolution;
	}

	auto Film::set_pixel(unsigned int x, unsigned int y, Vector3f p) ->void
	{
		if (x >= _resolution.x || y >= _resolution.y) {
			log(Status::fatal, "Index out of range");
		}

		auto index= y * _resolution.x + x;
		pixels[index].r = p.x;
		pixels[index].g = p.y;
		pixels[index].b = p.z;
	}


	auto Film::reset_resolution(Point2i res)->void
	{
		_resolution = res;
		pixels = std::make_unique<Pixel[]>(_resolution.x *_resolution.y);
	}

}