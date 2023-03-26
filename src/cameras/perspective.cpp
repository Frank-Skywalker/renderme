#include "perspective.hpp"

#include <core/log.hpp>

#include <imgui/imgui.h>


namespace renderme
{
	Perspective_Camera::Perspective_Camera()
	{
		update_camera_transforms();
	}

	Perspective_Camera::Perspective_Camera(glm::vec3 position, float yaw, float pitch, float fov)
	{
		config.position = position;
		config.yaw = yaw;
		config.pitch = pitch;
		config.fov = fov;
		update_camera_transforms();
	}

	auto Perspective_Camera::gl_draw(Shader const& shader) const noexcept -> void
	{
		shader.set_uniform_mat4("view", config.view.m);
		shader.set_uniform_mat4("projection", config.projection.m);
	}

	auto Perspective_Camera::generate_ray(glm::vec2 const& p) const noexcept -> Ray
	{
		// Ensure that p is in NDC space
		if (p.x < -1 || p.x > 1 || p.y < -1 || p.y > 1) {
			log(Status::fatal, "Invalid ray generate");
		}

		//auto origin = config.projection.transform_point(glm::vec3(0.f, 0.f, 0.f));
		//Ray local_ray(glm::vec3(p.x, p.y, origin.z), glm::vec3(0.f, 0.f, 1.f), config.near, config.far);
		//return camera_to_world.transform_ray(local_ray);

		auto front_scale = config.near;
		auto up_scale = front_scale * std::tanf( glm::radians(config.fov) / 2.f);
		auto right_scale = up_scale * config.aspect;
		
		auto ray_direction = config.front * front_scale + config.up * up_scale * p.y + config.right * right_scale * p.x;
		ray_direction = glm::normalize(ray_direction);
		return Ray(config.position, ray_direction, config.near, config.far);
	}

	auto Perspective_Camera::imgui_config() ->void
	{
		auto modified=false;
		modified |= ImGui::DragFloat3("Position", glm::value_ptr(config.position));
		modified |= ImGui::DragFloat3("World Up", glm::value_ptr(config.world_up));
		modified |= ImGui::DragFloat("Yaw", &config.yaw);
		modified |= ImGui::DragFloat("Pitch", &config.pitch);
		modified |= ImGui::DragFloat("Zoom", &config.fov);

		modified |= ImGui::DragFloat("Near", &config.near);
		modified |= ImGui::DragFloat("Far", &config.far);

		if (modified) {
			update_camera_transforms();
		}

		// Just display values, doesn't allow modification
		ImGui::Separator();
		auto front = config.front;
		ImGui::DragFloat3("Front", glm::value_ptr(front));
		auto up = config.up;
		ImGui::DragFloat3("Up", glm::value_ptr(up));
		auto right = config.right;
		ImGui::DragFloat3("Right", glm::value_ptr(right));

		ImGui::Separator();
		auto aspect = config.aspect;
		ImGui::DragFloat("Aspect", &aspect);

		ImGui::Separator();
		ImGui::DragFloat("Move Speed", &config.move_speed);
		ImGui::DragFloat("Cursor Speed", &config.cursor_speed);
		ImGui::DragFloat("Scroll Speed", &config.scroll_speed);
	}

	auto Perspective_Camera::process_keyboard(Camera_Movement move, float delta_time)->void
	{
		switch (move) {
			case Camera_Movement::forward:
				config.position += config.front * delta_time * config.move_speed;
				break;
			case Camera_Movement::backward:
				config.position -= config.front * delta_time * config.move_speed;
				break;
			case Camera_Movement::right:
				config.position += config.right * delta_time * config.move_speed;
				break;
			case Camera_Movement::left:
				config.position -= config.right * delta_time * config.move_speed;
				break;
			case Camera_Movement::up:
				config.position += config.world_up * delta_time * config.move_speed;
				break;
			case Camera_Movement::down:
				config.position -= config.world_up * delta_time * config.move_speed;
				break;
		}

		update_camera_transforms();
	}

	auto Perspective_Camera::process_cursor(float xdelta, float ydelta)->void
	{
		config.yaw += xdelta * config.cursor_speed;

		// Reversed since y-coordinates go from bottom to top
		config.pitch -= ydelta * config.cursor_speed;

		update_camera_transforms();
	}

	auto Perspective_Camera::process_scroll(float delta)->void
	{
		config.fov -= config.scroll_speed * delta;

		update_camera_transforms();
	}


	auto Perspective_Camera::check_config() -> void
	{
		// pitch
		if (config.pitch > 89) {
			config.pitch = 89;
		}
		if (config.pitch < -89) {
			config.pitch = -89;
		}

		// fov
		if (config.fov < 1) {
			config.fov = 1;
		}
		if (config.fov > 180) {
			config.fov = 180;
		}
	}

	auto Perspective_Camera::reset_aspect(float aspect)->void
	{
		config.aspect = aspect;
		update_camera_transforms();
	}


	auto Perspective_Camera::update_camera_transforms() ->void
	{
		check_config();

		// Calculate the new front vector
		config.front.x = glm::cos(glm::radians(config.yaw)) * glm::cos(glm::radians(config.pitch));
		config.front.y = glm::sin(glm::radians(config.pitch));
		config.front.z = glm::sin(glm::radians(config.yaw)) * glm::cos(glm::radians(config.pitch));
		config.front = glm::normalize(config.front);

		// Re-calculate the Right and Up vector
		// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		config.right= glm::normalize(glm::cross(config.front, config.world_up));
		config.up = glm::normalize(glm::cross(config.right, config.front));

		//Recalculate Transforms
		config.view = glm::lookAt(config.position, config.position + config.front, config.up);
		config.projection = glm::perspective(glm::radians(config.fov), config.aspect, config.near, config.far);

		// Recalculate camera transforms
		world_to_camera = config.projection * config.view;
		camera_to_world = world_to_camera.inverse();
	}


}
