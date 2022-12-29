#pragma once
#include "util.hpp"
#include "transform.hpp"
#include "shader.hpp"

namespace renderme
{
	enum struct Camera_Movement
	{
		forward,
		backward,
		left,
		right
	};

	struct Camera: Only_Movable
	{
		virtual auto gl_draw(Shader const& shader) const noexcept -> void;
		virtual auto generate_ray() const noexcept ->Float = 0;
		virtual auto imgui_config() ->void = 0;
		virtual auto process_keyboard_input(Camera_Movement move, float delta_time)->void = 0;
		virtual auto process_mouse_input(Float xoffset, Float yoffset)->void = 0;

		Transform camera_to_world;
		Transform world_to_camera;
	};
}