#include "film.hpp"
#include "log.hpp"

#include <gl/glew.h>
#include <stb/stb_image_write.h>

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

	auto Film::clear_color() const noexcept -> glm::vec3 const&
	{
		return _clear_color;
	}

	auto Film::pixel_of(glm::uvec2 id) const noexcept-> glm::vec3 const&
	{
		if (id.x >= _resolution.x || id.y >= _resolution.y) {
			log(Status::fatal, "Index out of range");
		}
		auto index = id.y * _resolution.x + id.x;
		return pixels[index];
	}

	auto Film::pixel_of(unsigned int x, unsigned int y) const noexcept -> glm::vec3 const&
	{
		if (x >= _resolution.x || y >= _resolution.y) {
			log(Status::fatal, "Index out of range");
		}
		auto index = y * _resolution.x + x;
		return pixels[index];

	}

	auto Film::set_clear_color(glm::vec3 const& color) -> void
	{
		_clear_color = color;
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
		std::fill(pixels.get(), pixels.get() + _resolution.x * _resolution.y, _clear_color);
	}

	auto Film::hash() const noexcept-> std::size_t
	{
		return Hasher().hash(_resolution).hash(_clear_color).value();
	}

	auto Film::export_to_file(Runtime_Path path) const noexcept -> void
	{
		std::vector<unsigned char> write_buffer(_resolution.x * _resolution.y * 3);

		auto clamp_min = glm::vec3(0.f, 0.f, 0.f);
		auto clamp_max = glm::vec3(1.f, 1.f, 1.f);
		for (auto i = 0; i < _resolution.x * _resolution.y; ++i) {
			auto color = glm::fclamp(pixels[i], clamp_min, clamp_max);
			write_buffer[i * 3] = color.x * 255;
			write_buffer[i * 3 + 1] = color.y * 255;
			write_buffer[i * 3 + 2] = color.z * 255;
		}

		stbi_flip_vertically_on_write(true);
		stbi_write_bmp(path.full_path().c_str(), _resolution.x, _resolution.y, 3, write_buffer.data());
	}

}