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
		build_data_structures(camera, scene, film);
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




	auto ZBuffer_Integrator::build_data_structures(Camera const* camera, Scene const& scene, Film* film)->void
	{
		for (auto& primitive: scene.gl_draw_primitives) {

			//Get Triangle_Mesh
			if (typeid(*primitive) == typeid(Shape_Primitive)){
				auto shape_primitive = dynamic_cast<Shape_Primitive const*>(primitive.get());
				if (typeid(*shape_primitive->shape) == typeid(Triangle_Mesh)) {
					auto triangle_mesh = dynamic_cast<Triangle_Mesh const*>(shape_primitive->shape.get());

					// Start dealing with each Triangle_Mesh

					// Do transformations for all positions
					std::vector<glm::vec3> positions;
					for (auto const& model_pos : triangle_mesh->positions) {

						auto model_space_to_ndc = [&] (glm::vec3 const& model)->glm::vec3 {
							auto ndc = model;
							// From Model Space to World Space
							if (triangle_mesh->object_to_world != nullptr) {
								ndc = triangle_mesh->object_to_world->transform_point(ndc);
							}
							// From World Space to NDC
							ndc = camera->world_to_camera.transform_point(ndc);
							return ndc;
						};

						auto ndc_to_screen_space = [] (glm::vec3 const& ndc, glm::uvec2 const& resolution)->glm::vec3 {
							auto x = (ndc.x + 1.0f) / 2.0f * static_cast<float>(resolution.x);
							auto y = (ndc.y + 1.0f) / 2.0f * static_cast<float>(resolution.y);
							auto z = (ndc.z + 1.0f) / 2.0f;
							return glm::vec3(x, y, z);
						};

						auto position = model_space_to_ndc(model_pos);
						positions.push_back(ndc_to_screen_space(position, film->resolution()));
					}


					// Deal with each face
					for (auto const& face : triangle_mesh->faces) {
						
						auto compute_plane_equation = [&] (glm::uvec3 const& face) -> glm::vec4 {
							// ax+by+cz+d=0
							auto vec0 = positions[face[1]] - positions[face[0]];
							auto vec1 = positions[face[2]] - positions[face[1]];
							auto normal = glm::normalize(glm::cross(vec0, vec1));
							// normal = (a,b,c)
							auto equation = glm::vec4(normal, 0);

							// Calculate last coefficient
							// d=-ax0-bx0-cz0
							equation.w = -equation.x * positions[face[0]].x - equation.y * positions[face[0]].y - equation.z * positions[face[0]].z;
							return equation;
						};

						// Compute equation for this polygon
						auto equation = compute_plane_equation(face);

						auto discard_face = [&] (glm::uvec3 const& face) -> bool {
							// No Clip at all
							// Discard the face that is not in the Screen space (0,0,0) -> (resolution.x, resolution.y, 1)
							for (auto i = 0u; i < face.length(); ++i) {
								auto x = static_cast<int>(positions[face[i]].x);
								auto y = static_cast<int>(positions[face[i]].y);
								auto z = positions[face[i]].z;
								if (
									x < 0 || x >= static_cast<int>(film->resolution().x) ||
									y < 0 || y >= static_cast<int>(film->resolution().y) ||
									z < 0 || z > 1
									) {
									return true;
								}
							}
							// Discard the face if it's perpendicular to XOY plane,
                            // in which case its normal is parallel to the XOY plane
							if (std::fabsf(equation.z) < EPSILON) {
								return true;
							}
							return false;
						};
						
						if (discard_face(face)) {
							std::cout << "danger" << std::endl;
							continue;
						}

						int polygon_maxy = std::numeric_limits<int>::min();
						int polygon_miny = std::numeric_limits<int>::max();
						Float sum_z = 0;

						// Deal with each edge
						for (auto i = 0u; i < face.length(); ++i) {

							// Convert to integer based screen point
							auto point0 = positions[face[i]];
							auto point1 = positions[face[(i + 1) % face.length()]];

							// Discard horizontal edges
							// Horizontal edges are already draw when dealing with non-horizontal edges
							if (point0.y == point1.y) {
								continue;
							}

							// Make sure that point0.y > point1.y
							if (point0.y < point1.y) {
								std::swap(point0, point1);
							}

							// Build an edge and push into edge table
							Edge edge;
							edge.dy = static_cast<int>(point0.y) - static_cast<int>(point1.y);
							edge.x = point0.x;
							edge.dxdy = -(point0.x - point1.x) / (point0.y - point1.y);

							edge_table[point0.y].push_back(std::move(edge));

							// Update polygon y
							polygon_maxy = std::max(polygon_maxy, static_cast<int>(point0.y));
							polygon_miny = std::min(polygon_miny, static_cast<int>(point1.y));
							sum_z += point0.z;
						}

						// Build a polygon and push into polygon table
						assert(polygon_maxy > polygon_miny);
						Polygon polygon;
						polygon.id = polygon_id++;
						polygon.equation = equation;
						polygon.dy = polygon_maxy - polygon_miny;
						polygon.color = glm::vec3(sum_z / static_cast<float>(face.length()));

						polygon_table[polygon_maxy].push_back(std::move(polygon));
					}

				}
			}
		}
	}

	auto ZBuffer_Integrator::perform_zbuffer(Film* film)->void
	{
		// 7. Find the largest and smallest Y values for the vertices of each polygon
		// 8. Sort on Ymax to create a table with the following entries:


	}

	auto ZBuffer_Integrator::clean_data_structures()->void
	{
		polygon_id = 0;
		polygon_table.clear();
		edge_table.clear();
		active_polygon_table.clear();
		active_edge_pair_table.clear();
	}

}