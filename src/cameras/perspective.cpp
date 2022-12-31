#include "perspective.hpp"

#include <core/math.hpp>

#include <cmath>
#include <imgui/imgui.h>

namespace renderme
{
	auto Perspective_Camera::gl_draw(Shader const& shader) const noexcept -> void
	{
		shader.set_uniform_mat4("view", config.view.matrix());
		shader.set_uniform_mat4("projection", config.projection.matrix());
	}

	auto Perspective_Camera::generate_ray() const noexcept ->Float
	{
		return 0;
	}
	auto Perspective_Camera::imgui_config() ->void
	{
		auto modified=false;
		modified |= ImGui::DragFloat3("Position", &config.position[0]);
		modified |= ImGui::DragFloat3("World Up", &config.world_up[0]);
		modified |= ImGui::DragFloat("Yaw", &config.yaw);
		modified |= ImGui::DragFloat("Pitch", &config.pitch);
		modified |= ImGui::DragFloat("Zoom", &config.zoom);

		if (modified) {
			update_camera_transforms();
		}

		ImGui::Separator();
		auto front = config.front;
		ImGui::DragFloat3("Front", &front[0]);
		auto up = config.up;
		ImGui::DragFloat3("Up", &up[0]);
		auto right = config.right;
		ImGui::DragFloat3("Right", &right[0]);


		ImGui::Separator();
		ImGui::DragFloat("Move Speed", &config.move_speed);
		ImGui::DragFloat("Cursor Speed", &config.cursor_speed);
		ImGui::DragFloat("Scroll Speed", &config.scroll_speed);
	}

	auto Perspective_Camera::process_keyboard(Camera_Movement move, Float delta_time)->void
	{
		switch (move) {
			case Camera_Movement::forward:
				config.position += config.front * delta_time * config.move_speed;
				break;
			case Camera_Movement::backward:
				config.position -= config.front * delta_time * config.move_speed;
				break;
			case Camera_Movement::left:
				config.position -= config.right * delta_time * config.move_speed;
				break;
			case Camera_Movement::right:
				config.position += config.right * delta_time * config.move_speed;
				break;
		}

		update_camera_transforms();
	}

	auto Perspective_Camera::process_cursor(Float xdelta, Float ydelta)->void
	{
		config.yaw += xdelta * config.cursor_speed;
		config.pitch += ydelta * config.cursor_speed;

		if (config.pitch > 89) {
			config.pitch = 89;
		}
		if (config.pitch < -89) {
			config.pitch = -89;
		}
		update_camera_transforms();
	}

	auto Perspective_Camera::process_scroll(Float delta)->void
	{
		config.zoom -= config.scroll_speed * delta;

		if (config.zoom < 1) {
			config.zoom = 1;
		}
		if (config.zoom > 45) {
			config.zoom = 45;
		}
		update_camera_transforms();
	}

	auto Perspective_Camera::update_camera_transforms() ->void
	{
		//Calculate the new front vector
		config.front.x = std::cos(radians(config.yaw)) * std::cos(radians(config.pitch));
		config.front.y = std::sin(radians(config.pitch));
		config.front.z = std::sin(radians(config.yaw)) * std::cos(radians(config.pitch));
		config.front = normalize(config.front);

		//Re-calculate the Right and Up vector
		//Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		config.right= normalize(cross(config.front, config.world_up));  
		config.up = normalize(cross(config.right, config.front));

		//Recalculate Transforms
		//config.view = translate(Vector3f(config.position.x, config.position.y, config.position.z));
		config.view = lookat(config.position, config.position + config.front, config.up);
		config.projection = perspective(config.zoom, 0.1f, 100.0f);

		world_to_camera = config.projection * config.view;
		camera_to_world = world_to_camera.inverse();
	}


}