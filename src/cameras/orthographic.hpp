#pragma once
#include <core/camera.hpp>

#include <glm/glm-all.hpp>

namespace renderme
{

	struct Orthographic_Camera final: Camera
	{

		struct Config
		{
			glm::vec3 position;
			glm::vec3 direction;
			glm::vec3 up;

			float speed{1.0f};
		};

		auto gl_draw(Shader const& shader) const noexcept -> void;
		auto generate_ray(glm::vec2 const& p) const noexcept -> Ray override;
		auto imgui_config() ->void;

		auto process_keyboard(Camera_Movement move, float delta_time)->void;
		auto process_cursor(float xdelta, float ydelta)->void;
		auto process_scroll(float delta)->void;
		auto reset_aspect(float aspect)->void;

	private:
		auto update_camera_transforms() ->void;
		Config config;
	};
}
