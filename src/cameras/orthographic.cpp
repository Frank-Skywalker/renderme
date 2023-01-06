#include "orthographic.hpp"
#include <imgui/imgui.h>

namespace renderme
{

	auto Orthographic_Camera::gl_draw(Shader const& shader) const noexcept -> void
	{

	}


	auto Orthographic_Camera::generate_ray() const noexcept ->Float
	{
		return 0;
	}
	auto Orthographic_Camera::imgui_config() ->void
	{
		ImGui::DragFloat3("Position", glm::value_ptr(config.position));
		ImGui::DragFloat3("Direction", glm::value_ptr(config.direction));
		ImGui::DragFloat3("Up", glm::value_ptr(config.up));
	}

	auto Orthographic_Camera::process_keyboard(Camera_Movement move, Float delta_time)->void
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

	auto Orthographic_Camera::process_cursor(Float xoffset, Float yoffset)->void
	{

	}

	auto Orthographic_Camera::process_scroll(Float offset)->void
	{
		//Empty, no zoom
	}


	auto Orthographic_Camera::update_camera_transforms() ->void
	{

	}

}