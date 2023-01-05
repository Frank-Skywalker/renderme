#include "zbuffer.hpp"

#include<cameras/perspective.hpp>

#include <imgui/imgui.h>

namespace renderme
{

	ZBuffer_Integrator::ZBuffer_Integrator(Film* film)
		:Integrator(std::make_unique<Perspective_Camera>(), std::make_unique<Shader>("src/shaders/phong.vert.glsl", "src/shaders/phong.frag.glsl")),
		film(film)
	{}

	auto ZBuffer_Integrator::gl_draw(Scene const& scene) const noexcept -> void
	{
		//Be sure to enable shader before setting uniforms
		shader->use();
		camera->gl_draw(*shader);
		scene.gl_draw(*shader);
	}

	auto ZBuffer_Integrator::render(Scene const& scene) const noexcept -> void
	{
		//build data structures
		//perform zbuffer
	}

	auto ZBuffer_Integrator::imgui_config() ->void
	{
		if (ImGui::TreeNode("Camera")) {
			camera->imgui_config();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Shader")) {
			shader->imgui_config();
			ImGui::TreePop();
		}
	}
}