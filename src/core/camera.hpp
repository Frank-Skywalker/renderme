#pragma once
#include "util.hpp"
#include "transform.hpp"

namespace renderme
{
	struct Camera: Only_Movable
	{
		virtual auto generate_ray() const noexcept ->Float = 0;
		virtual auto imgui_config() ->void = 0;

		Transform camera_to_world;
	};
}