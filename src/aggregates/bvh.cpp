#include "bvh.hpp"

namespace renderme
{

	auto build_bvh_recursively(
		int begin, int end, 
		std::vector<Primitive const*>& primitives, 
		std::vector<Primitive const*>& ordered_primitives, 
		Strategy strategy, 
		int max_primitives_per_node
	) -> std::unique_ptr<BVH_Node>
	{
		// Calculate bvh node bounds
		Bounds3f bounds;
		for (auto i = begin; i < end; ++i) {
			bounds.eat(primitives[i]->world_bounds());
		}

		auto num_primitives = end - begin;

		if (num_primitives == 1) {
			// Create a leaf node
			ordered_primitives.push_back(primitives[begin]);
			return std::make_unique<BVH_Node>(begin, 1, bounds);
		}


	}


	BVH::BVH(std::vector<std::unique_ptr<Primitive>> primivites, unsigned int max_primitives_per_node, Strategy strategy)
		:primitives{ std::move(primitives) }, max_primitives_per_node{max_primitives_per_node}, strategy{strategy}
	{
		if (primitives.empty())
			return;

		ordered_primitives.reserve(primitives.size());

		std::vector<Primitive const*> primitives_info;
		for (auto const& prim : primitives) {
			primitives_info.push_back(prim.get());
		}

		bvh_tree = build_bvh_recursively(0, primitives.size(), primitives_info, ordered_primitives, strategy, max_primitives_per_node);
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
		return Bounds3f();
	}




}