#include "zbuffer.hpp"

#include<core/log.hpp>
#include<cameras/perspective.hpp>
#include<shapes/triangle.hpp>

#include <imgui/imgui.h>

#include <algorithm>

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

		// CAUTION!!!:
		// Be sure to UNUSE shader after rendering
		// Or there may be bugs in afterwards rendering
		shader->unuse();
	}

	auto ZBuffer_Integrator::render(Camera const* camera, Scene const& scene, Film* film) -> void
	{
		build_polygon_table(camera, scene, film);
		perform_zbuffer(film);
		clean_polygon_table();
	}

	auto ZBuffer_Integrator::imgui_config() ->void
	{
		if (ImGui::TreeNode("Shader")) {
			shader->imgui_config();
			ImGui::TreePop();
		}
	}


	auto solve_equation_for_z(glm::vec4 const& equation, Float x, Float y) -> Float
	{
		//ax+by+cz+d=0
		return (-equation.w - equation.x * x - equation.y * y) / equation.z;
	}

	auto ZBuffer_Integrator::build_polygon_table(Camera const* camera, Scene const& scene, Film* film)->void
	{
		//Resize polygon table
		polygon_table.resize(film->resolution().y);

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

						// From Model Space to NDC
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

						// From NDC to Screen Space
						auto ndc_to_screen_space = [] (glm::vec3 const& ndc, glm::uvec2 const& resolution)->glm::vec3 {
							auto x = (ndc.x + 1.0f) / 2.0f * static_cast<float>(resolution.x);
							auto y = (ndc.y + 1.0f) / 2.0f * static_cast<float>(resolution.y);

							// CAUTION!!!: Flip z here
							// Projection matrix will transfer from right-handed World Space to left-handed NDC
							// But we want to stay in right-handed coordinate space
							// So we should flip z back
							auto z = ( - ndc.z + 1.0f) / 2.0f;

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
							continue;
						}
						
						// Start building polygon
						int polygon_ymax = std::numeric_limits<int>::min();
						int polygon_ymin = std::numeric_limits<int>::max();
						Float sum_z = 0;
						std::vector<Edge> edges;

						// Deal with each edge
						for (auto i = 0u; i < face.length(); ++i) {
							auto point0 = positions[face[i]];
							auto point1 = positions[face[(i + 1) % face.length()]];

							// Make sure that point0.y > point1.y
							if (point0.y < point1.y) {
								std::swap(point0, point1);
							}

							// Update polygon y
							polygon_ymax = std::max(polygon_ymax, static_cast<int>(point0.y));
							polygon_ymin = std::min(polygon_ymin, static_cast<int>(point1.y));
							sum_z += point0.z;

							// Discard horizontal edges
							// Horizontal edges are already draw when dealing with non-horizontal edges
							if (static_cast<int>(point0.y) == static_cast<int>(point1.y)) {
								continue;
							}

							// Build an edge
							Edge edge;
							edge.ymax = static_cast<int>(point0.y);
							edge.dxdy = -(point0.x - point1.x) / (point0.y - point1.y);
							edge.dy = static_cast<int>(point0.y) - static_cast<int>(point1.y);
							edge.x = point0.x;
							edge.z = solve_equation_for_z(equation, point0.x, point0.y);

							edges.push_back(std::move(edge));
						}

						assert(polygon_ymax >= polygon_ymin);
						auto color = (triangle_mesh->normals[face[0]] + triangle_mesh->normals[face[1]] + triangle_mesh->normals[face[2]]) / 3.0f;

						// First compares ymax
                        // Then compares x
                        // Last compares dxdy
						auto edge_comparator = [] (Edge const& lhs, Edge const& rhs)->bool {
							if (lhs.ymax == rhs.ymax) {
								if (lhs.x == rhs.x) {
									return lhs.dxdy < rhs.dxdy;
								}
								return lhs.x < rhs.x;
							}
							return lhs.ymax > rhs.ymax;
						};
						// Sort edges
						std::sort(edges.begin(), edges.end(), edge_comparator);

						// Build a polygon and push into polygon table
						Polygon polygon;
						polygon.equation = equation;
						polygon.dzdx = -equation.x / equation.z;
						polygon.dzdy = -equation.y / equation.z;
						polygon.color = color;
						polygon.dy = polygon_ymax - polygon_ymin;
						polygon.edges = std::move(edges);

						polygon_table[polygon_ymax].push_back(std::move(polygon));
					}

				}
			}
		}
	}


	auto ZBuffer_Integrator::perform_zbuffer(Film* film)->void
	{
		// Scan line
		auto zbuffer = std::make_unique<Float[]>(film->resolution().x);
		Active_Polygon_List active_polygon_list;

		for (auto scany = static_cast<int>(film->resolution().y) - 1 ; scany >= 0; --scany) {

			// Clear zbuffer
			std::fill(zbuffer.get(), zbuffer.get() + film->resolution().x, std::numeric_limits<Float>::min());

			// Add first-time scanned polygons (polygon_ymax == yscan) to active polygon table
			for(auto &polygon :polygon_table[scany]) {
				active_polygon_list.push_back(&polygon);
			}

			// Scan through each polygon
			for (auto polygon : active_polygon_list) {
				
				// Add new edges
				for (auto &edge : polygon->edges) {
					if (edge.ymax == scany) {
						polygon->active_edge_list.push_back(&edge);
					}
				}

				for (auto iter = polygon->active_edge_list.begin(); iter != polygon->active_edge_list.end();) {

					// Get edge pair
					auto left = *iter;
					++iter;
					auto right = *iter;
					++iter;
					
					// Update zbuffer and frame buffer
					auto z = left->z;
					for (auto x = static_cast<int>(left->x);x <= static_cast<int>(right->x);++x) {
						if (z >= zbuffer[x]) {
							zbuffer[x] = z;
							film->set_pixel(glm::uvec2(x, scany), polygon->color);
						}
						z += polygon->dzdx;
					}

					// Update active edges
					--left->dy;
					left->x += left->dxdy;
					left->z += polygon->dzdx * left->dxdy + polygon->dzdy;

					--right->dy;
					right->x += right->dxdy;
					right->z += polygon->dzdx * right->dxdy + polygon->dzdy;
				}

				// Remove active edges if dy==0
				for (auto iter = polygon->active_edge_list.begin(); iter != polygon->active_edge_list.end(); ) {
					if ((*iter)->dy <= 0) {
						iter=polygon->active_edge_list.erase(iter);
					} else {
						++iter;
					}
				}

				// Update active polygon
				--polygon->dy;

			}// End of polygon

			// Remove active polygons if dy==0
			for (auto iter = active_polygon_list.begin(); iter != active_polygon_list.end(); ) {
				if ((*iter)->dy <= 0) {
					iter = active_polygon_list.erase(iter);
				}
				else {
					++iter;
				}
			}

		}// End of scany

	}

	auto ZBuffer_Integrator::clean_polygon_table()->void
	{
		polygon_table.clear();
	}

}