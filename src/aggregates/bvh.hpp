#pragma once
#include <core/primitive.hpp>
#include <vector>

namespace renderme
{
	enum struct Strategy
	{
		equal,
		sah,		// Surface Area Heuristic
	};


	struct BVH_Node
	{
		Bounds3f bounds;
		Axis split_axis{Axis::invalid};
		int offset{0};
		int num_primitives{0};

		std::unique_ptr<BVH_Node> left;
		std::unique_ptr<BVH_Node> right;

		// Leaf node
		BVH_Node(int offset, int num_primitives, Bounds3f const& bounds)
			:offset{offset}, num_primitives{num_primitives}, bounds{bounds}
		{}

		// Interior node
		BVH_Node(Axis split_axis, std::unique_ptr<BVH_Node> left, std::unique_ptr<BVH_Node> right)
			:split_axis{ split_axis }, bounds{left->bounds, right->bounds}, left {std::move(left)}, right{ std::move(right) }
		{
			assert(split_axis != Axis::invalid);
		}
	};

	struct BVH final: Aggregate
	{
		BVH(std::vector<std::unique_ptr<Primitive>> primivites, Strategy strategy = Strategy::sah);

		auto gl_draw(Shader const& shader) const noexcept -> void override;
		auto intersect(Ray const& ray, Interaction* interaction) const noexcept ->bool override;
		auto intersect_shadow(Ray const& ray) const noexcept ->bool override;
		auto imgui_config() const noexcept ->void override;

		auto world_bounds() const noexcept -> Bounds3f const& override;


	private:
		auto build_bvh_recursively(int begin, int end) -> std::unique_ptr<BVH_Node>;
		auto build_bvh_vao()->void;

	private:
		std::vector<std::unique_ptr<Primitive>> primitives;
		Strategy strategy{ Strategy::sah };

		std::vector<Primitive const*> ordered_primitives;
		std::unique_ptr<BVH_Node> bvh_tree;

		int num_bvh_nodes{0};
		unsigned int vao;
	};

}