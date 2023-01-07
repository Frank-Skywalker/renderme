#pragma once
#include "type.hpp"
#include "util.hpp"
#include<glm/glm-all.hpp>
#include<memory>

namespace renderme
{
	struct Film final: Non_Transferable
	{
		Film()=default;
		Film(glm::uvec2 resolution);

		auto reset_resolution(glm::uvec2 const& reslotion)->void;
		auto set_pixel(unsigned int x, unsigned int y, glm::vec3 const& p) ->void;
		auto set_pixel(glm::uvec2 id, glm::vec3 const& p) ->void;
		auto resolution() const noexcept ->glm::uvec2 const&;

		auto gl_display() const noexcept ->void;
		auto clear(glm::vec3 const& color) ->void;
		
	private:
		std::unique_ptr<glm::vec3[]> pixels;
		glm::uvec2 _resolution;
	};
}