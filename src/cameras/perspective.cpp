#include "perspective.hpp"

#include <imgui/imgui.h>

namespace renderme
{
	auto Perspective_Camera::generate_ray() const noexcept ->Float
	{
		return 0;
	}
	auto Perspective_Camera::imgui_config() ->void
	{
		ImGui::DragFloat3("Position", (float*)&config.position.x);
		ImGui::DragFloat3("Direction", (float*)&config.direction.x);
		ImGui::DragFloat3("Up", (float*)&config.up.x);
	}

	auto Perspective_Camera::process_keyboard_input(Camera_Movement move, float delta_time)->void
	{
		switch (move) {
			case Camera_Movement::forward:
				//config.position += delta_time * config.speed
				break;
			case Camera_Movement::backward:
				break;
			case Camera_Movement::left:
				break;
			case Camera_Movement::right:
				break;
		}
	}

	auto Perspective_Camera::process_mouse_input(Float xoffset, Float yoffset)->void
	{

	}


}