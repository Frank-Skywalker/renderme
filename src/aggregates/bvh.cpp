#include "bvh.hpp"

#include <core/log.hpp>

#include <GL/glew.h>

#include <algorithm>
#include <stack>
#include <array>
#include <iostream>

#define RR_BVH_SAH_APROXIMATE_LIMIT 2
#define RR_BVH_SAH_BUCKET_NUM 12

namespace renderme
{

	auto BVH::build_bvh_recursively(int begin, int end) -> std::unique_ptr<BVH_Node>
	{
		// Calculate bvh node bounds
		Bounds3f bounds;
		Bounds3f center_bounds;
		for (auto i = begin; i < end; ++i)
		{
			bounds.eat(ordered_primitives[i]->world_bounds());
			center_bounds.eat(ordered_primitives[i]->world_bounds().center());
		}
		//std::cout << "center_bounds min: " << center_bounds.pmin << std::endl;
		//std::cout << "center_bounds max: " << center_bounds.pmax << std::endl;
		//std::cout << "bounds min: " << bounds.pmin << std::endl;
		//std::cout << "bounds max: " << bounds.pmax << std::endl;
		auto num_primitives = end - begin;

		// Create a leaf node if only one primitive is left
		if (num_primitives <= 1) {
			return std::make_unique<BVH_Node>(begin, num_primitives, bounds);
		}

		auto axis = center_bounds.max_extent();
		// Create a leaf node if all primitives' center overlaps on the maximum extent axis
		if (center_bounds.pmin[int(axis)] == center_bounds.pmax[int(axis)]) {
			return std::make_unique<BVH_Node>(begin, num_primitives, bounds);
		}

		auto mid = (begin + end) / 2;

		auto equal_partition = [&]()->void {
			mid = (begin + end) / 2;
			std::nth_element(&ordered_primitives[begin], &ordered_primitives[mid], &ordered_primitives[end - 1] + 1,
				[axis](Primitive const* lhs, Primitive const* rhs)->bool {
					return lhs->world_bounds().center()[int(axis)] < rhs->world_bounds().center()[int(axis)];
				}
			);
		};

		// Partition primitives depending on strategy
		switch (strategy) {
		case Strategy::equal:
			equal_partition();
			break;

		case Strategy::sah:
			// If num primitives less than approximate limit
			// Use approximate SAH strategy whitch is Strategy::equal
			if (num_primitives <= RR_BVH_SAH_APROXIMATE_LIMIT) {
				equal_partition();
			}
			else {
				// <count, bb>
				std::array<std::pair<int, Bounds3f>, RR_BVH_SAH_BUCKET_NUM> buckets;

				// Insert all primitives into buckets
				for (auto i = begin; i < end; ++i) {
					// Calculate which bucket this primitive center falls in
					auto offset = center_bounds.offset(ordered_primitives[i]->world_bounds().center());
					//std::cout << "world_bounds center: " << ordered_primitives[i]->world_bounds().center() << std::endl;
					//std::cout << "offset: " << offset << std::endl;
					int n = RR_BVH_SAH_BUCKET_NUM * offset[int(axis)];
					if (n >= RR_BVH_SAH_BUCKET_NUM)
						n = RR_BVH_SAH_BUCKET_NUM - 1;

					//std::cout << "n: " << n << std::endl;

					++buckets[n].first;
					buckets[n].second.eat(ordered_primitives[i]->world_bounds());
				}

				//for (auto i = 0; i < RR_BVH_SAH_BUCKET_NUM; ++i) {
				//	std::cout << "bucket" << i << ": " << buckets[i].first << std::endl;
				//	std::cout << "min" << i << ": " << buckets[i].second.pmin << std::endl;
				//	std::cout << "max" << i << ": " << buckets[i].second.pmax << std::endl;
				//}

				// Compute costs when partitioning before each bucket
				// Find minimum cost partition at the same time
				auto min_cost = std::numeric_limits<float>::max();
				int min_split_bucket = 0;

				for (auto i = 1; i < RR_BVH_SAH_BUCKET_NUM; ++i) {
					int left_count = 0;
					int right_count = 0;
					Bounds3f left_bounds;
					Bounds3f right_bounds;

					for (auto j = 0; j < i; ++j) {
						left_count += buckets[j].first;
						left_bounds.eat(buckets[j].second);
					}
					for (auto j = i; j < RR_BVH_SAH_BUCKET_NUM; ++j) {
						right_count += buckets[j].first;
						right_bounds.eat(buckets[j].second);
					}

					auto cost = 1 + (left_count * left_bounds.surface_area() + right_count * right_bounds.surface_area()) / bounds.surface_area();
					//if (bounds.surface_area() == 0) {
					//	cost = 1;
					//}

					//std::cout << "left surface area: " << left_bounds.surface_area() << std::endl;
					//std::cout << "right surface area: " << right_bounds.surface_area() << std::endl;
					//std::cout << "bounds surface area: " << bounds.surface_area() << std::endl;
					//std::cout << "cost: " << cost << std::endl;
					//std::cout << "min cost: " << min_cost << std::endl;
					if (cost < min_cost) {
						min_cost = cost;
						min_split_bucket = i;
					}
				}

				//std::cout << "min_split: "<<min_split_bucket << std::endl;

				float leaf_cost = num_primitives;
				// Create leaf node
				if (min_cost >= leaf_cost && num_primitives <= max_primitives_per_node) {
					return std::make_unique<BVH_Node>(begin, num_primitives, bounds);
				}
				// Perform partition
				else {
					auto mid_ptr = std::partition(
						&ordered_primitives[begin], &ordered_primitives[end - 1] + 1,
						[&](Primitive const*& p)->bool {
							// Calculate which bucket this primitive center falls in
							auto offset = center_bounds.offset(p->world_bounds().center());
							int n = RR_BVH_SAH_BUCKET_NUM * offset[int(axis)];
							if (n >= RR_BVH_SAH_BUCKET_NUM)
								n = RR_BVH_SAH_BUCKET_NUM - 1;

							return n < min_split_bucket;
						}
					);
					// Set mid
					mid = mid_ptr - &ordered_primitives[0];
				}
			}
			break;

		default:
			log(Status::fatal, "No such strategy");
		}

		
		return std::make_unique<BVH_Node>(
			axis,
			build_bvh_recursively(begin, mid),
			build_bvh_recursively(mid, end)
		);
	}


	auto BVH::build_bvh_vao()->void
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::uvec2> indices;

		std::stack<BVH_Node const*> stack;

		stack.push(bvh_tree.get());

		while (!stack.empty()) {
			auto node = stack.top();
			++num_bvh_nodes;
			stack.pop();

			auto i = positions.size();
			indices.push_back(glm::uvec2(i, i + 1));
			indices.push_back(glm::uvec2(i + 2, i + 3));
			indices.push_back(glm::uvec2(i + 4, i + 5));
			indices.push_back(glm::uvec2(i + 6, i + 7));

			indices.push_back(glm::uvec2(i, i + 4));
			indices.push_back(glm::uvec2(i + 1, i + 5));
			indices.push_back(glm::uvec2(i + 2, i + 6));
			indices.push_back(glm::uvec2(i + 3, i + 7));

			indices.push_back(glm::uvec2(i, i + 2));
			indices.push_back(glm::uvec2(i + 1, i + 3));
			indices.push_back(glm::uvec2(i + 4, i + 6));
			indices.push_back(glm::uvec2(i + 5, i + 7));

			positions.push_back(glm::vec3(node->bounds.pmin.x, node->bounds.pmin.y, node->bounds.pmin.z));
			positions.push_back(glm::vec3(node->bounds.pmin.x, node->bounds.pmin.y, node->bounds.pmax.z));
			positions.push_back(glm::vec3(node->bounds.pmin.x, node->bounds.pmax.y, node->bounds.pmin.z));
			positions.push_back(glm::vec3(node->bounds.pmin.x, node->bounds.pmax.y, node->bounds.pmax.z));
			positions.push_back(glm::vec3(node->bounds.pmax.x, node->bounds.pmin.y, node->bounds.pmin.z));
			positions.push_back(glm::vec3(node->bounds.pmax.x, node->bounds.pmin.y, node->bounds.pmax.z));
			positions.push_back(glm::vec3(node->bounds.pmax.x, node->bounds.pmax.y, node->bounds.pmin.z));
			positions.push_back(glm::vec3(node->bounds.pmax.x, node->bounds.pmax.y, node->bounds.pmax.z));

			normals.push_back(glm::vec3(1, 1, 1));
			normals.push_back(glm::vec3(1, 1, 1));
			normals.push_back(glm::vec3(1, 1, 1));
			normals.push_back(glm::vec3(1, 1, 1));
			normals.push_back(glm::vec3(1, 1, 1));
			normals.push_back(glm::vec3(1, 1, 1));
			normals.push_back(glm::vec3(1, 1, 1));
			normals.push_back(glm::vec3(1, 1, 1));


			if (node->num_primitives == 0) {
				stack.push(node->right.get());
				stack.push(node->left.get());
			}
		}


		// create buffers/arrays
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;

		/////Vertex/////
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

		/////Normal/////
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

		/////EBO/////
		unsigned int ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec2), indices.data(), GL_STATIC_DRAW);

		//Unbind vao
		glBindVertexArray(0);
	}


	BVH::BVH(std::vector<std::unique_ptr<Primitive>> _primitives, unsigned int _max_primitives_per_node, Strategy strategy)
		:primitives{ std::move(_primitives) }, max_primitives_per_node{_max_primitives_per_node}, strategy{strategy}
	{
		ordered_primitives.reserve(primitives.size());
		for (auto const& prim : primitives) {
			ordered_primitives.push_back(prim.get());
		}

		bvh_tree = build_bvh_recursively(0, ordered_primitives.size());
		build_bvh_vao();
	}


	auto BVH::gl_draw(Shader const& shader) const noexcept -> void
	{
		shader.set_uniform_mat4("model", glm::identity<glm::mat4>());
		glBindVertexArray(vao);
		glDrawElements(GL_LINES, num_bvh_nodes * 12, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	auto BVH::intersect(Ray const& ray, Interaction* interaction) const noexcept ->bool
	{
		auto hit = false;
		std::stack<BVH_Node const*> stack;

		stack.push(bvh_tree.get());

		while (!stack.empty()) {
			auto node = stack.top();
			stack.pop();

			// Check if ray intersect with BVH node's bounding box
			if (node->bounds.intersect(ray)) {
				// Leaf node
				if (node->num_primitives > 0) {
					for (auto i = 0; i < node->num_primitives; ++i)
						hit |= ordered_primitives[node->offset + i]->intersect(ray, interaction);
				}
				// Interior node
				else {
					// Visit near half tree first
					if (ray.direction[int(node->split_axis)] > 0) {
						stack.push(node->right.get());
						// Left is first visited
						stack.push(node->left.get());
					}
					else {
						stack.push(node->left.get());
						// Right is first visited
						stack.push(node->right.get());
					}
				}
			}
		}

		return hit;
	}

	auto BVH::intersect_shadow(Ray const& ray) const noexcept ->bool
	{
		std::stack<BVH_Node const*> stack;

		stack.push(bvh_tree.get());

		while (!stack.empty()) {
			auto node = stack.top();
			stack.pop();

			// Check if ray intersect with BVH node's bounding box
			if (node->bounds.intersect(ray)) {
				// Leaf node
				if (node->num_primitives > 0) {
					for (auto i = 0; i < node->num_primitives; ++i){
						if (ordered_primitives[node->offset + i]->intersect_shadow(ray)) {
							return true;
						}
					}
				}
				// Interior node
				else {
					// Visit near half tree first
					if (ray.direction[int(node->split_axis)] > 0) {
						stack.push(node->right.get());
						// Left is first visited
						stack.push(node->left.get());
					}
					else {
						stack.push(node->left.get());
						// Right is first visited
						stack.push(node->right.get());
					}
				}
			}
		}
		return false;
	}

	auto BVH::imgui_config() const noexcept ->void
	{

	}

	auto BVH::world_bounds() const noexcept -> Bounds3f const&
	{
		return bvh_tree->bounds;
	}




}