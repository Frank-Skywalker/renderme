#pragma once 
#include <core/camera.hpp>
namespace renderme
{

	struct Orthographic_Camera final: Camera
	{

		struct Config
		{
			Point3f position;
			Vector3f direction;
			Vector3f up;

			Float speed{1.0f};
		};

		auto generate_ray() const noexcept ->Float;
		auto imgui_config() ->void;

		auto process_keyboard(Camera_Movement move, Float delta_time)->void;
		auto process_cursor(Float xoffset, Float yoffset)->void;
		auto process_scroll(Float offset)->void;

	private:
		auto update_camera_transforms() ->void;
		Config config;
	};
}