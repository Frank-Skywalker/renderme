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

		auto gl_draw(Shader const& shader) const noexcept -> void override;
		auto generate_ray(glm::vec2 const& p) const noexcept -> Ray override;
		auto imgui_config() ->void override;

		auto process_keyboard(Camera_Movement move, float delta_time)->void override;
		auto process_cursor(float xdelta, float ydelta)->void override;
		auto process_scroll(float delta)->void override;
		auto reset_aspect(float aspect)->void override;
		auto hash() const noexcept-> std::size_t override;


	private:
		auto update_camera_transforms() ->void;
		Config config;
	};
}
