#include "zbuffer.hpp"

#include<cameras/perspective.hpp>

#include <imgui/imgui.h>

namespace renderme
{

	ZBuffer_Integrator::ZBuffer_Integrator()
		:Integrator(std::make_unique<Shader>("src/shaders/phong.vert.glsl", "src/shaders/phong.frag.glsl"))
	{}

	auto ZBuffer_Integrator::gl_draw(Camera const* camera, Scene const& scene) const noexcept -> void
	{
		//Be sure to enable shader before setting uniforms
		shader->use();
		camera->gl_draw(*shader);
		scene.gl_draw(*shader);
	}

	auto ZBuffer_Integrator::render(Camera const* camera, Scene const& scene, Film* film) const noexcept -> void
	{
		//build data structures
		//perform zbuffer
	}

	auto ZBuffer_Integrator::imgui_config() ->void
	{
		if (ImGui::TreeNode("Shader")) {
			shader->imgui_config();
			ImGui::TreePop();
		}
	}
}