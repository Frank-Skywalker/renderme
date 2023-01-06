#pragma once

#include "film.hpp"
#include "log.hpp"

#include <gl/glew.h>

namespace renderme
{

	Film::Film(glm::uvec2 res)
		:_resolution{res}
	{
		pixels = std::make_unique<Pixel[]>(_resolution.x * _resolution.y);
	}


	auto Film::resolution() const noexcept ->glm::uvec2 const&
	{
		return _resolution;
	}

	auto Film::set_pixel(unsigned int x, unsigned int y, Pixel const& p) ->void
	{
		if (x >= _resolution.x || y >= _resolution.y) {
			log(Status::fatal, "Index out of range");
		}

		auto index= y * _resolution.x + x;
		pixels[index] = p;
	}

	auto Film::set_pixel(glm::uvec2 id, Pixel const& p) ->void
	{
		set_pixel(id.x, id.y, p);
	}


	auto Film::reset_resolution(glm::uvec2 const& res)->void
	{
		_resolution = res;
		pixels = std::make_unique<Pixel[]>(_resolution.x *_resolution.y);
	}


	auto Film::gl_display() const noexcept ->void
	{
		glDrawPixels(_resolution.x, _resolution.y, GL_RGB, GL_FLOAT, pixels.get());
	}

}