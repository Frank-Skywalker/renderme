#include "perspective-camera.hpp"

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
		ImGui::DragFloat3("Position", (float*)&config.direction.x);
		ImGui::DragFloat3("Position", (float*)&config.up.x);
	}

}