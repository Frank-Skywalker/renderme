#pragma once
#include "util.hpp"
#include "transform.hpp"
#include "shader.hpp"
#include "ray.hpp"

namespace renderme
{
	enum struct Camera_Movement
	{
		forward,
		backward,
		left,
		right,
		up,
		down
	};

	struct Camera: Only_Movable
	{
		virtual auto gl_draw(Shader const& shader) const noexcept -> void = 0;
		virtual auto generate_ray(glm::vec2 const& p) const noexcept -> Ray = 0;
		virtual auto imgui_config() ->void = 0;
		virtual auto process_keyboard(Camera_Movement move, float delta_time)->void = 0;
		virtual auto process_cursor(float xoffset, float yoffset)->void = 0;
		virtual auto process_scroll(float offset)->void = 0;
		virtual auto reset_aspect(float aspect)->void = 0;
		virtual auto hash() const noexcept-> std::size_t = 0;

		Transform camera_to_world;
		Transform world_to_camera;
	};
}
