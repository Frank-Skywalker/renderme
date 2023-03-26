#include "film.hpp"
#include "log.hpp"

#include <gl/glew.h>

#include<algorithm>

namespace renderme
{

	Film::Film(glm::uvec2 res)
		:_resolution{res}
	{
		pixels = std::make_unique<glm::vec3[]>(_resolution.x * _resolution.y);
	}


	auto Film::resolution() const noexcept ->glm::uvec2 const&
	{
		return _resolution;
	}

	auto Film::set_clear_color(glm::vec3 const& color) -> void
	{
		clear_color = color;
	}


	auto Film::set_pixel(unsigned int x, unsigned int y, glm::vec3 const& p) ->void
	{
		if (x >= _resolution.x || y >= _resolution.y) {
			log(Status::fatal, "Index out of range");
		}

		auto index= y * _resolution.x + x;
		pixels[index] = p;
	}

	auto Film::set_pixel(glm::uvec2 id, glm::vec3 const& p) ->void
	{
		set_pixel(id.x, id.y, p);
	}


	auto Film::reset_resolution(glm::uvec2 const& res)->void
	{
		_resolution = res;
		pixels = std::make_unique<glm::vec3[]>(_resolution.x *_resolution.y);
	}


	auto Film::gl_display() const noexcept ->void
	{
		//Be sure NOT to set the raster position back to (0,0) before glDrawPixels
		//glRasterPos4i(0, 0, 0, 1);
		glDrawPixels(_resolution.x, _resolution.y, GL_RGB, GL_FLOAT, pixels.get());
	}

	auto Film::clear() ->void
	{
		std::fill(pixels.get(), pixels.get() + _resolution.x * _resolution.y, clear_color);
	}

}