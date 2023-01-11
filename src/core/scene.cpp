#include "scene.hpp"
#include <imgui/imgui.h>
namespace renderme
{

	Scene::Scene(std::string _name, std::vector<std::unique_ptr<Transform>> _transforms,
		std::vector<std::unique_ptr<Primitive>> _gl_draw_primitives,
		std::vector<std::unique_ptr<Primitive>> _render_primitives,
		std::vector<std::unique_ptr<Light>> _lights)
		:name{std::move(_name)},
		gl_draw_primitives{std::move(_gl_draw_primitives)},
		render_primitives{std::move(_render_primitives)},
		lights{std::move(_lights)}, transforms{std::move(_transforms)}
	{}


	auto Scene::gl_draw(Shader const& shader) const noexcept -> void
	{
		for (auto const& primitive : gl_draw_primitives) {
			primitive->gl_draw(shader);
		}
	}


	auto Scene::intersect() const noexcept->bool
	{
		auto result = false;
		for (auto const& primitive : render_primitives) {
			result |= primitive->intersect();
		}
		return result;
	}


	auto Scene::intersect_shadow() const noexcept->bool
	{
		auto result = false;
		for (auto const& primitive : render_primitives) {
			result |= primitive->intersect_shadow();
		}
		return result;
	}

	auto Scene::imgui_config()->void
	{
		auto i = 0;
		for (auto& primitive : gl_draw_primitives) {
			if (ImGui::TreeNode(("Primitive"+std::to_string(i++)).c_str())) {
				primitive->imgui_config();
				ImGui::TreePop();
			}
		}
	}
}