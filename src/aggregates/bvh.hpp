#pragma once
#include <core/primitive.hpp>
#include <vector>
namespace renderme
{
	enum struct Strategy
	{
		sah,
	};

	enum struct Axis
	{
		invalid,
		x,
		y,
		z,
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
		BVH(std::vector<std::unique_ptr<Primitive>> primivites, unsigned int max_primitives_per_node = 1, Strategy strategy = Strategy::sah);

		auto gl_draw(Shader const& shader) const noexcept -> void override;
		auto intersect() const noexcept ->bool override;
		auto intersect_shadow() const noexcept ->bool override;
		auto imgui_config() const noexcept ->void override;

		auto world_bounds() const noexcept -> Bounds3f const& override;


	private:
		std::vector<std::unique_ptr<Primitive>> primitives;
		unsigned int max_primitives_per_node{ 1 };
		Strategy strategy{ Strategy::sah };

		std::vector<Primitive const*> ordered_primitives;
		std::unique_ptr<BVH_Node> bvh_tree;
	};

}