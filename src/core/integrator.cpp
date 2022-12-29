#include "integrator.hpp"

#include <imgui/imgui.h>

namespace renderme
{

	Integrator::Integrator(std::unique_ptr<Camera> camera, std::unique_ptr<Shader> shader)
		:camera{std::move(camera)}, shader{std::move(shader)}
	{
	}



	Sample_Integrator::Sample_Integrator(std::unique_ptr<Camera> camera, std::unique_ptr<Shader> shader)
		:Integrator(std::move(camera),std::move(shader))
	{
	}


	auto Sample_Integrator::gl_draw(Scene const& scene) const noexcept -> void
	{
		//Be sure to enable shader before setting uniforms
		shader->use();
		camera->gl_draw(*shader);
		scene.gl_draw(*shader);
	}

	auto Sample_Integrator::render(Scene const& scene) const noexcept -> void
	{
		scene.intersect();
	}

	auto Sample_Integrator::imgui_config() ->void
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