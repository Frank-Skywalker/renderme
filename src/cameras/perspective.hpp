#pragma once
#include <core/camera.hpp>
#include <core/vector.hpp>
namespace renderme
{
	struct Perspective_Camera final: Camera
	{
		Perspective_Camera();
		struct Config
		{
			//Can be set
			Point3f position{0, 0, 0};
			Vector3f world_up{0, 1, 0};
			Float yaw{-90};
			Float pitch{0};
			Float zoom{45.f};

			//Calculated from above
			Vector3f up;
			Vector3f front;
			Vector3f right;

			Transform view;
			Transform projection;

			//Speed metadatas
			Float move_speed{1.0f};
			Float cursor_speed{0.1f};
			Float scroll_speed{1.0f};
		};


		//Perspective_Camera(Point3f pos, );
		auto gl_draw(Shader const& shader) const noexcept -> void;
		auto generate_ray() const noexcept ->Float;
		auto imgui_config() ->void;
		auto process_keyboard(Camera_Movement move, Float delta_time)->void;
		auto process_cursor(Float xdelta, Float ydelta)->void;
		auto process_scroll(Float delta)->void;


	private:
		auto update_camera_transforms() ->void;
		Config config;

	};
}