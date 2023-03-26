#include "orthographic.hpp"

#include <core/log.hpp>

#include <imgui/imgui.h>

namespace renderme
{

	auto Orthographic_Camera::gl_draw(Shader const& shader) const noexcept -> void
	{

	}


	auto Orthographic_Camera::generate_ray(glm::vec2 const& p) const noexcept -> Ray
	{
		if (p.x < 0 || p.x>1 || p.y < 0 || p.y>1) {
			log(Status::fatal, "Invalid ray generate");
		}

		return Ray(glm::vec3(), glm::vec3());
	}

	auto Orthographic_Camera::imgui_config() ->void
	{
		ImGui::DragFloat3("Position", glm::value_ptr(config.position));
		ImGui::DragFloat3("Direction", glm::value_ptr(config.direction));
		ImGui::DragFloat3("Up", glm::value_ptr(config.up));
	}

	auto Orthographic_Camera::process_keyboard(Camera_Movement move, float delta_time)->void
	{
		switch (move) {
			case Camera_Movement::forward:
				break;
			case Camera_Movement::backward:
				break;
			case Camera_Movement::left:
				break;
			case Camera_Movement::right:
				break;
		}
	}

	auto Orthographic_Camera::process_cursor(float xoffset, float yoffset)->void
	{

	}

	auto Orthographic_Camera::process_scroll(float offset)->void
	{
		//Empty, no zoom
	}


	auto Orthographic_Camera::reset_aspect(float aspect)->void
	{

	}


	auto Orthographic_Camera::update_camera_transforms() ->void
	{

	}

	auto Orthographic_Camera::hash() const noexcept-> std::size_t
	{
		return 0;
	}


}
