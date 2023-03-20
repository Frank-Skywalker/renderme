#include "bvh.hpp"

#include <core/log.hpp>

#include <GL/glew.h>
#include <algorithm>

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
				std::vector<std::pair<int, Bounds3f>> buckets;

				// Insert all primitives into buckets
				for (auto i = begin; i < end; ++i) {
					// Calculate which bucket this primitive center falls in
					auto offset = center_bounds.offset(ordered_primitives[i]->world_bounds().center());
					int n = RR_BVH_SAH_BUCKET_NUM * offset[int(axis)];
					if (n >= RR_BVH_SAH_BUCKET_NUM)
						n = RR_BVH_SAH_BUCKET_NUM - 1;

					++buckets[n].first;
					buckets[n].second.eat(ordered_primitives[i]->world_bounds());
				}

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
					if (cost < min_cost) {
						min_cost = cost;
						min_split_bucket = i;
					}
				}

				float leaf_cost = num_primitives;
				// Create leaf node
				if (min_cost >= leaf_cost && num_primitives <= max_primitives_per_node) {
					return std::make_unique<BVH_Node>(begin, num_primitives, bounds);
				}
				// Perform partition
				else {
					auto mid_ptr = std::partition(
						&ordered_primitives[begin], &ordered_primitives[end - 1] + 1,
						[&](Primitive const* p)->bool {
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


	BVH::BVH(std::vector<std::unique_ptr<Primitive>> primivites, unsigned int max_primitives_per_node, Strategy strategy)
		:primitives{ std::move(primitives) }, max_primitives_per_node{max_primitives_per_node}, strategy{strategy}
	{
		ordered_primitives.reserve(primitives.size());
		for (auto const& prim : primitives) {
			ordered_primitives.push_back(prim.get());
		}

		bvh_tree = build_bvh_recursively(0, ordered_primitives.size());
	}


	auto BVH::gl_draw(Shader const& shader) const noexcept -> void
	{
	}

	auto BVH::intersect() const noexcept ->bool
	{
		return false;
	}

	auto BVH::intersect_shadow() const noexcept ->bool
	{
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