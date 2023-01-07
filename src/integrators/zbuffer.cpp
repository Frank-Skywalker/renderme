#include "zbuffer.hpp"

#include<core/log.hpp>
#include<cameras/perspective.hpp>
#include<shapes/triangle.hpp>

#include <imgui/imgui.h>

namespace renderme
{

	ZBuffer_Integrator::ZBuffer_Integrator()
		:Integrator(std::make_unique<Shader>("src/shaders/phong.vert.glsl", "src/shaders/phong.frag.glsl"))
	{}

	auto ZBuffer_Integrator::gl_draw(Camera const* camera, Scene const& scene) -> void
	{
		//Be sure to enable shader before setting uniforms
		shader->use();
		camera->gl_draw(*shader);
		scene.gl_draw(*shader);
	}

	auto ZBuffer_Integrator::render(Camera const* camera, Scene const& scene, Film* film) -> void
	{
		build_data_structures(camera, scene);
		perform_zbuffer(film);
		clean_data_structures();
	}

	auto ZBuffer_Integrator::imgui_config() ->void
	{
		if (ImGui::TreeNode("Shader")) {
			shader->imgui_config();
			ImGui::TreePop();
		}
	}



	auto ZBuffer_Integrator::build_data_structures(Camera const* camera, Scene const& scene)->void
	{
		for (auto& primitive: scene.gl_draw_primitives) {
			if (typeid(*primitive) == typeid(Shape_Primitive)){
				auto shape_primitive = dynamic_cast<Shape_Primitive const*>(primitive.get());
				if (typeid(*shape_primitive->shape) == typeid(Triangle_Mesh)) {
					auto triangle_mesh = dynamic_cast<Triangle_Mesh const*>(shape_primitive->shape.get());

					// 1.Transform positions to NDC Coordinates
					auto positions = triangle_mesh->positions;
					for (auto& position : positions) {
						//From Model Space to World Space
						if (triangle_mesh->object_to_world != nullptr) {
							position=triangle_mesh->object_to_world->transform_point(position);
						}
						//From World Space to NDC
						position = camera->world_to_camera.transform_point(position);
					}

					// 2.Compute plane equation of all polygons
					auto compute_plane_equation = [&] (glm::uvec3 const& face) -> glm::vec4 {
						glm::vec4 equation;
						auto edge_vec1 = positions[face[2]] - positions[face[1]];
						auto edge_vec2 = positions[face[0]] - positions[face[1]];

						equation[0] = edge_vec1.y * edge_vec2.z - edge_vec1.z * edge_vec2.y;
						equation[1] = edge_vec1.z * edge_vec2.x - edge_vec1.x * edge_vec2.z;
						equation[2] = edge_vec1.x * edge_vec2.y - edge_vec1.y * edge_vec2.x;

						// Normalize coefficients
						auto sum = std::fabsf(equation[0]) + std::fabsf(equation[1]) + std::fabsf(equation[2]);
						if (sum < EPSILON) {
							log(Status::fatal, "sum is zero");
						}
						equation[0] /= sum;
						equation[1] /= sum;
						equation[2] /= sum;

						// Calculate last coefficient
						equation[3] = -equation[0] * positions[face[0]].x - equation[1] * positions[face[0]].y - equation[2] * positions[face[0]].z;

						return equation;
					};
					
					for (auto const& face : triangle_mesh->faces) {
						polygon_id++;
						compute_plane_equation(face);
					}



				}
			}
		}
	}

	auto ZBuffer_Integrator::perform_zbuffer(Film* film)->void
	{

	}

	auto ZBuffer_Integrator::clean_data_structures()->void
	{

	}

}