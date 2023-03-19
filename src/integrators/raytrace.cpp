#include "raytrace.hpp"

#include <core/shader.hpp>

#include <imgui/imgui.h>

namespace renderme
{
	Raytrace_Integrator::Raytrace_Integrator()
		:Integrator(std::make_unique<Shader>("src/shaders/normal.vert.glsl", "src/shaders/normal.frag.glsl"))
	{

	}

	auto Raytrace_Integrator::gl_draw(Camera const* camera, Scene const& scene) -> void
	{
		//Be sure to enable shader before setting uniforms
		shader->use();
		camera->gl_draw(*shader);
		scene.gl_draw(*shader);

		// CAUTION!!!
		// Be sure to UNUSE shader after rendering
		// Or there may be bugs in afterwards rendering
		shader->unuse();
	}

	auto Raytrace_Integrator::render(Camera const* camera, Scene const& scene, Film* film) -> void
	{

	}

	auto Raytrace_Integrator::imgui_config() ->void
	{
		if (ImGui::TreeNode("Shader")) {
			shader->imgui_config();
			ImGui::TreePop();
		}
	}
}