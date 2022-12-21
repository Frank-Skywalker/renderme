#pragma once
#include <core/camera.hpp>
#include <core/vector.hpp>
namespace renderme
{
	struct Perspective_Camera final: Camera
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
		auto process_keyboard_input(Camera_Movement move, float delta_time)->void;
		auto process_mouse_input(Float xoffset, Float yoffset)->void;

		Config config;
	};
}