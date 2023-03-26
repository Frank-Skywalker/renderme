#pragma once
#include <core/camera.hpp>

#include <glm/glm-all.hpp>

namespace renderme
{
	struct Perspective_Camera final: Camera
	{
		Perspective_Camera();
		Perspective_Camera(glm::vec3 position, float yaw, float pitch, float fov);
		struct Config
		{
			//Can be set
			glm::vec3 position{0, 0, 0};
			glm::vec3 world_up{0, 1, 0};
			float yaw{-90};
			float pitch{0};
			float fov{45.f};

			float near{0.1f};
			float far{10000.0f};

			// Fed in
			float aspect{ 1.0f };

			//Calculated from above
			glm::vec3 up;
			glm::vec3 front;
			glm::vec3 right;

			Transform view;
			Transform projection;

			//Speed metadatas
			float move_speed{2.0f};
			float cursor_speed{0.1f};
			float scroll_speed{1.0f};
		};


		//Perspective_Camera(Point3f pos, );
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
		auto check_config() -> void;

	public:
		Config config;
	};
}
