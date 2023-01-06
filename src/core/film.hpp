#pragma once
#include "type.hpp"
#include "util.hpp"
#include<glm/glm-all.hpp>
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
		Film(glm::uvec2 resolution);

		auto reset_resolution(glm::uvec2 const& reslotion)->void;
		auto set_pixel(unsigned int x, unsigned int y, Pixel const& p) ->void;
		auto set_pixel(glm::uvec2 id, Pixel const& p) ->void;
		auto resolution() const noexcept ->glm::uvec2 const&;

		auto gl_display() const noexcept ->void;
		
	private:
		std::unique_ptr<Pixel[]> pixels;
		glm::uvec2 _resolution;
	};
}