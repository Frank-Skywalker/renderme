#pragma once
#include "util.hpp"
#include "runtime-path.hpp"
#include<glm/glm-all.hpp>
#include<memory>

namespace renderme
{
	struct Film final: Non_Transferable
	{
		Film()=default;
		Film(glm::uvec2 resolution);

		auto clear_color() const noexcept -> glm::vec3 const&;
		auto resolution() const noexcept ->glm::uvec2 const&;
		auto pixel_of(glm::uvec2 id) const noexcept-> glm::vec3 const&;
		auto pixel_of(unsigned int x, unsigned int y) const noexcept -> glm::vec3 const&;
		auto export_to_file(Runtime_Path path) const noexcept -> void;

		auto reset_resolution(glm::uvec2 const& reslotion)->void;
		auto set_pixel(unsigned int x, unsigned int y, glm::vec3 const& p) ->void;
		auto set_pixel(glm::uvec2 id, glm::vec3 const& p) ->void;
		auto set_clear_color(glm::vec3 const& color) -> void;

		auto gl_display() const noexcept ->void;
		auto clear() ->void;
		auto hash() const noexcept-> std::size_t;


	private:
		std::unique_ptr<glm::vec3[]> pixels;
		glm::uvec2 _resolution;
		glm::vec3 _clear_color{ 0.f, 0.f, 0.f };
	};
}
