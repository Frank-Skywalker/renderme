#pragma once
#include <core/camera.hpp>

#include <glm/glm-all.hpp>

namespace renderme
{
	struct Perspective_Camera final: Camera
	{
		Perspective_Camera();
		struct Config
		{
			//Can be set
			glm::vec3 position{0, 0, 0};
			glm::vec3 world_up{0, 1, 0};
			float yaw{-90};
			float pitch{0};
			float fov{45.f};

			//Calculated from above
			glm::vec3 up;
			glm::vec3 front;
			glm::vec3 right;

			Transform view;
			Transform projection;

			//Speed metadatas
			float move_speed{1.0f};
			float cursor_speed{0.1f};
			float scroll_speed{1.0f};
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
		auto check_config() -> void;
		Config config;

	};
}