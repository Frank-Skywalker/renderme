#include "scene.hpp"
#include <imgui/imgui.h>

// TODO: remove these
#include <shapes/triangle.hpp>
#include <aggregates/bvh.hpp>

namespace renderme
{

	Scene::Scene(
		std::string _name,
		std::vector<std::unique_ptr<Transform>> _transforms,
		std::vector<std::unique_ptr<Texture>> _textures,
		std::vector<std::unique_ptr<Shape>> _shapes,
		std::vector<std::unique_ptr<Material>> _materials,
		std::vector<std::unique_ptr<Primitive>> _primitives,
		std::vector<std::unique_ptr<Light>> _lights
	)
		:name{ std::move(_name) },
		transforms{ std::move(_transforms) },
		textures{ std::move(_textures) },
		shapes{ std::move(_shapes) },
		materials{ std::move(_materials) },
		primitives{ std::move(_primitives) },
		lights{ std::move(_lights) }
	{}


	auto Scene::gl_draw(Shader const& shader) const noexcept -> void
	{
		for (auto const& primitive : primitives) {
			if (typeid(*primitive) == typeid(BVH)) {
				if (config.show_aggregate) {
					primitive->gl_draw(shader);
				}
			}
			else {
				if (config.show_primitives) {
					primitive->gl_draw(shader);
				}
			}
		}
	}


	auto Scene::intersect(Ray const& ray, Interaction* interaction) const noexcept->bool
	{
		auto result = false;
		for (auto const& primitive : primitives) {
			result |= primitive->intersect(ray, interaction);
		}
		return result;
	}

	auto Scene::intersect_shadow(Ray const& ray) const noexcept->bool
	{
		auto result = false;
		for (auto const& primitive : primitives) {
			result |= primitive->intersect_shadow(ray);
		}
		return result;
	}

	auto Scene::imgui_config()->void
	{
		ImGui::Checkbox("Show Aggreagte", &config.show_aggregate);
		ImGui::Checkbox("Show Aggreagte", &config.show_primitives);

		//auto i = 0;
		auto face_count = 0;
		auto vertex_count = 0;

		for (auto& primitive : primitives) {
			//Get Triangle_Mesh
			if (typeid(*primitive) == typeid(Shape_Primitive)) {
				auto shape_primitive = dynamic_cast<Shape_Primitive const*>(primitive.get());
				if (typeid(*shape_primitive->shape) == typeid(Triangle_Mesh)) {
					auto triangle_mesh = dynamic_cast<Triangle_Mesh const*>(shape_primitive->shape);

					vertex_count += triangle_mesh->positions.size();
					face_count += triangle_mesh->faces.size();
				}
			}
			//if (ImGui::TreeNode(("Primitive"+std::to_string(i++)).c_str())) {
			//	primitive->imgui_config();
			//	ImGui::TreePop();
			//}
		}
		ImGui::Text("Vertex Count: %d", vertex_count);
		ImGui::Text("Face Count: %d", face_count);

	}
}
